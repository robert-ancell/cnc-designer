/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (CncPlan, cnc_plan, CNC, PLAN, GObject)

typedef struct
{
    gdouble x0, y0;
    gdouble x1, y1;
} CncLine;

CncPlan   *cnc_plan_new       (void);

CncLine   *cnc_plan_add_line  (CncPlan *plan);

GPtrArray *cnc_plan_get_lines (CncPlan *plan);

G_END_DECLS
