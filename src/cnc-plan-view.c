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
};

G_DEFINE_TYPE (CncPlanView, cnc_plan_view, GTK_TYPE_DRAWING_AREA)

static gboolean
cnc_plan_view_draw (GtkWidget *widget, cairo_t *cr)
{
    CncPlanView *self = CNC_PLAN_VIEW (widget);

    cairo_set_source_rgb (cr, 1.0, 0, 0);
    cairo_paint (cr);

    return TRUE;
}

static void
cnc_plan_view_class_init (CncPlanViewClass *klass)
{
    GTK_WIDGET_CLASS (klass)->draw = cnc_plan_view_draw;
}

static void
cnc_plan_view_init (CncPlanView *self)
{
}
