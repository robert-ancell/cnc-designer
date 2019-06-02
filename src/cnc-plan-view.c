/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "cnc-plan-view.h"

struct _CncPlanView
{
    GtkDrawingArea parent_instance;

    CncPlan *plan;
    CncLine *current_line;
};

G_DEFINE_TYPE (CncPlanView, cnc_plan_view, GTK_TYPE_DRAWING_AREA)

static gboolean
cnc_plan_view_draw (GtkWidget *widget, cairo_t *cr)
{
    CncPlanView *self = CNC_PLAN_VIEW (widget);

    cairo_set_source_rgb (cr, 1.0, 0, 0);
    cairo_paint (cr);

    if (self->plan == NULL)
        return TRUE;

    GPtrArray *lines = cnc_plan_get_lines (self->plan);
    for (guint i = 0; i < lines->len; i++) {
        CncLine *line = g_ptr_array_index (lines, i);
        cairo_move_to (cr, line->x0, line->y0);
        cairo_line_to (cr, line->x1, line->y1);
    }
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_stroke (cr);

    return TRUE;
}

static gboolean
cnc_plan_view_button_press_event (GtkWidget *widget, GdkEventButton *event)
{
    CncPlanView *self = CNC_PLAN_VIEW (widget);

    if (self->plan == NULL)
        return TRUE;

    if (self->current_line != NULL) {
        self->current_line->x1 = event->x;
        self->current_line->y1 = event->y;
        self->current_line = NULL;
    }
    else {
        self->current_line = cnc_plan_add_line (self->plan);
        self->current_line->x0 = event->x;
        self->current_line->y0 = event->y;
        self->current_line->x1 = event->x;
        self->current_line->y1 = event->y;
    }

    gtk_widget_queue_draw (GTK_WIDGET (self));

    return TRUE;
}

static gboolean
cnc_plan_view_motion_notify_event (GtkWidget *widget, GdkEventMotion *event)
{
    CncPlanView *self = CNC_PLAN_VIEW (widget);

    if (self->current_line != NULL) {
        self->current_line->x1 = event->x;
        self->current_line->y1 = event->y;
        gtk_widget_queue_draw (GTK_WIDGET (self));
    }

    return FALSE;
}

static void
cnc_plan_view_class_init (CncPlanViewClass *klass)
{
    GTK_WIDGET_CLASS (klass)->draw = cnc_plan_view_draw;
    GTK_WIDGET_CLASS (klass)->button_press_event = cnc_plan_view_button_press_event;
    GTK_WIDGET_CLASS (klass)->motion_notify_event = cnc_plan_view_motion_notify_event;
}

static void
cnc_plan_view_init (CncPlanView *self)
{
    gtk_widget_add_events (GTK_WIDGET (self), GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);
}

void
cnc_plan_view_set_plan (CncPlanView *self, CncPlan *plan)
{
    g_set_object (&self->plan, plan);
}
