/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "cnc-plan.h"

struct _CncPlan
{
    GObject parent_instance;

    GPtrArray *lines;
};

G_DEFINE_TYPE (CncPlan, cnc_plan, G_TYPE_OBJECT)

static CncLine *
cnc_line_new (void)
{
    CncLine *line = g_new0 (CncLine, 1);
    return line;
}

static void
cnc_line_free (CncLine *line)
{
    g_free (line);
}

static void
cnc_plan_dispose (GObject *object)
{
    CncPlan *self = CNC_PLAN (object);
    g_clear_pointer (&self->lines, g_ptr_array_unref);
}

static void
cnc_plan_class_init (CncPlanClass *klass G_GNUC_UNUSED)
{
    G_OBJECT_CLASS (klass)->dispose = cnc_plan_dispose;
}

static void
cnc_plan_init (CncPlan *self)
{
     self->lines = g_ptr_array_new_with_free_func ((GDestroyNotify) cnc_line_free);
}

CncPlan *
cnc_plan_new (void)
{
    return g_object_new (cnc_plan_get_type (), NULL);
}

CncLine *
cnc_plan_add_line (CncPlan *self)
{
    g_return_val_if_fail (CNC_IS_PLAN (self), NULL);
    CncLine *line = cnc_line_new ();
    g_ptr_array_add (self->lines, line);
    return line;
}

GPtrArray *
cnc_plan_get_lines (CncPlan *self)
{
    g_return_val_if_fail (CNC_IS_PLAN (self), NULL);
    return self->lines;
}
