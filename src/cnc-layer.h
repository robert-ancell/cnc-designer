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

#include "cnc-shape.h"

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (CncLayer, cnc_layer, CNC, LAYER, GObject)

CncLayer    *cnc_layer_new           (void);

CncLayer    *cnc_layer_new_from_json (JsonNode    *node);

JsonNode    *cnc_layer_to_json       (CncLayer    *layer);

void         cnc_layer_set_name      (CncLayer    *layer,
                                      const gchar *name);

const gchar *cnc_layer_get_name      (CncLayer    *layer);

CncShape    *cnc_layer_add_shape     (CncLayer    *layer);

GPtrArray   *cnc_layer_get_shapes    (CncLayer    *layer);

G_END_DECLS
