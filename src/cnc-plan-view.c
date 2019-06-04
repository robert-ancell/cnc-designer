/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "cnc-plan-view.h"
#include "cnc-etcher.h" // FIXME temp

struct _CncPlanView
{
    GtkDrawingArea parent_instance;

    CncPlan *plan;
    CncLine *current_line;
    gdouble scale;
    gdouble x_offset;
    gdouble y_offset;
    gdouble width;
    gdouble height;
};

G_DEFINE_TYPE (CncPlanView, cnc_plan_view, GTK_TYPE_DRAWING_AREA)

static void
screen_to_plan (CncPlanView *self, gdouble x, gdouble y, gdouble *plan_x, gdouble *plan_y)
{
    *plan_x = x / self->scale - self->x_offset;
    *plan_y = y / self->scale - self->y_offset;
}

static void
plan_to_screen (CncPlanView *self, gdouble x, gdouble y, gdouble *screen_x, gdouble *screen_y)
{
    *screen_x = (x + self->x_offset) * self->scale;
    *screen_y = (y + self->y_offset) * self->scale;
}

static gboolean
cnc_plan_view_draw (GtkWidget *widget, cairo_t *cr)
{
    CncPlanView *self = CNC_PLAN_VIEW (widget);

    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_paint (cr);

    /* Draw a grid */
    for (int x = 0; x <= self->width; x += 10) {
        gdouble x0, y0, x1, y1;
        plan_to_screen (self, x, 0, &x0, &y0);
        plan_to_screen (self, x, self->height, &x1, &y1);
        cairo_move_to (cr, x0 + 0.5, y0 + 0.5);
        cairo_line_to (cr, x1 + 0.5, y1 + 0.5);
    }
    for (int y = 0; y <= self->height; y += 10) {
        gdouble x0, y0, x1, y1;
        plan_to_screen (self, 0, y, &x0, &y0);
        plan_to_screen (self, self->width, y, &x1, &y1);
        cairo_move_to (cr, x0 + 0.5, y0 + 0.5);
        cairo_line_to (cr, x1 + 0.5, y1 + 0.5);
    }
    cairo_set_source_rgba (cr, 0, 0, 1, 0.5);
    cairo_stroke (cr);

    if (self->plan == NULL)
        return TRUE;

    GPtrArray *layers = cnc_plan_get_layers (self->plan);
    for (guint i = 0; i < layers->len; i++) {
        CncLayer *layer = g_ptr_array_index (layers, i);
        GPtrArray *shapes = cnc_layer_get_shapes (layer);
        for (guint j = 0; j < shapes->len; j++) {
            CncShape *shape = g_ptr_array_index (shapes, i);
            GPtrArray *lines = cnc_shape_get_lines (shape);
            for (guint k = 0; k < lines->len; k++) {
                CncLine *line = g_ptr_array_index (lines, k);
                gdouble x0, y0, x1, y1;
                plan_to_screen (self, line->x0, line->y0, &x0, &y0);
                plan_to_screen (self, line->x1, line->y1, &x1, &y1);
                cairo_move_to (cr, x0, y0);
                cairo_line_to (cr, x1, y1);
            }
        }
    }
    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_stroke (cr);

    return TRUE;
}

static void
save (CncPlanView *self)
{
    g_autoptr(JsonNode) root = cnc_plan_to_json (self->plan);
    g_autoptr(JsonGenerator) generator = json_generator_new ();
    json_generator_set_root (generator, root);
    g_autofree gchar *data = json_generator_to_data (generator, NULL);
    g_autoptr(GError) error = NULL;
    g_file_set_contents ("cnc-autosave.json", data, -1, &error);
}

static CncShape *
get_selected_shape (CncPlanView *self)
{
    if (self->plan == NULL)
        return NULL;

    GPtrArray *layers = cnc_plan_get_layers (self->plan);
    if (layers->len == 0)
        return NULL;
    CncLayer *layer = g_ptr_array_index (layers, 0);

    GPtrArray *shapes = cnc_layer_get_shapes (layer);
    if (shapes->len == 0) {
        CncShape *shape = cnc_layer_add_shape (layer);
        return shape;
    }

    return g_ptr_array_index (shapes, 0);
}

static gboolean
cnc_plan_view_button_press_event (GtkWidget *widget, GdkEventButton *event)
{
    CncPlanView *self = CNC_PLAN_VIEW (widget);

    CncShape *shape = get_selected_shape (self);
    if (shape == NULL)
        return TRUE;

    gdouble x, y;
    screen_to_plan (self, event->x, event->y, &x, &y);

    if (event->button == 1) {
        if (self->current_line != NULL) {
            self->current_line->x1 = x;
            self->current_line->y1 = y;
            // FIXME: do somewhere else
            save (self);
        }
        self->current_line = cnc_shape_add_line (shape);
        self->current_line->x0 = x;
        self->current_line->y0 = y;
        self->current_line->x1 = x;
        self->current_line->y1 = y;
    }
    else if (event->button == 3) {
        if (self->current_line != NULL) {
            cnc_shape_remove_line (shape, self->current_line);
            self->current_line = NULL;
        }
        // FIXME: temp
        g_autoptr(CncGcodeGenerator) generator = cnc_gcode_generator_new ();
        cnc_gcode_generator_add_comment (generator, "TEST");
        cnc_gcode_generator_add_distance_mode (generator, CNC_DISTANCE_MODE_ABSOLUTE);
        cnc_gcode_generator_add_units (generator, CNC_UNITS_MILLIMETER);
        cnc_gcode_generator_add_spindle_on (generator, CNC_SPINDLE_DIRECTION_CLOCKWISE, 0);
        g_autoptr(CncEtcher) etcher = cnc_etcher_new ();
        cnc_etcher_add_shape (etcher, shape);
        cnc_etcher_generate_gcode (etcher, generator);
        cnc_gcode_generator_add_end_of_program (generator);
        g_autofree gchar *gcode = cnc_gcode_generator_to_data (generator);
        g_printerr ("%s", gcode);
    }

    gtk_widget_queue_draw (GTK_WIDGET (self));

    return TRUE;
}

static gboolean
cnc_plan_view_scroll_event (GtkWidget *widget, GdkEventScroll *event)
{
    CncPlanView *self = CNC_PLAN_VIEW (widget);

    if (event->direction == GDK_SCROLL_UP) {
        self->scale *= 1.2;
    }
    else if (event->direction == GDK_SCROLL_DOWN) {
        self->scale /= 1.2;
    }

    gtk_widget_queue_draw (GTK_WIDGET (self));

    return TRUE;
}

static gboolean
cnc_plan_view_motion_notify_event (GtkWidget *widget, GdkEventMotion *event)
{
    CncPlanView *self = CNC_PLAN_VIEW (widget);

    gdouble x, y;
    screen_to_plan (self, event->x, event->y, &x, &y);

    if (self->current_line != NULL) {
        self->current_line->x1 = x;
        self->current_line->y1 = y;
        gtk_widget_queue_draw (GTK_WIDGET (self));
    }

    return FALSE;
}

static void
cnc_plan_view_class_init (CncPlanViewClass *klass)
{
    GTK_WIDGET_CLASS (klass)->draw = cnc_plan_view_draw;
    GTK_WIDGET_CLASS (klass)->button_press_event = cnc_plan_view_button_press_event;
    GTK_WIDGET_CLASS (klass)->scroll_event = cnc_plan_view_scroll_event;
    GTK_WIDGET_CLASS (klass)->motion_notify_event = cnc_plan_view_motion_notify_event;
}

static void
cnc_plan_view_init (CncPlanView *self)
{
    gtk_widget_add_events (GTK_WIDGET (self), GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_SCROLL_MASK | GDK_POINTER_MOTION_MASK);
    self->scale = 4.0;
    self->x_offset = 10.0;
    self->y_offset = 10.0;
    self->width = 300.0;
    self->height = 200.0;
}

void
cnc_plan_view_set_plan (CncPlanView *self, CncPlan *plan)
{
    g_set_object (&self->plan, plan);
}
