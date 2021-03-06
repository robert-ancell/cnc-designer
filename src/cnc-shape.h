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
#include <json-glib/json-glib.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (CncShape, cnc_shape, CNC, SHAPE, GObject)

typedef struct
{
    gdouble x0, y0;
    gdouble x1, y1;
} CncLine;

CncShape  *cnc_shape_new           (void);

CncShape  *cnc_shape_new_from_json (JsonNode *node);

JsonNode  *cnc_shape_to_json       (CncShape *shape);

CncLine   *cnc_shape_add_line      (CncShape *shape);

void       cnc_shape_remove_line   (CncShape *shape, CncLine *line);

GPtrArray *cnc_shape_get_lines     (CncShape *shape);

G_END_DECLS
