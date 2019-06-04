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

#include "cnc-layer.h"

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (CncPlan, cnc_plan, CNC, PLAN, GObject)

CncPlan   *cnc_plan_new           (void);

CncPlan   *cnc_plan_new_from_json (JsonNode *node);

JsonNode  *cnc_plan_to_json       (CncPlan  *plan);

CncLayer  *cnc_plan_add_layer     (CncPlan  *plan);

GPtrArray *cnc_plan_get_layers    (CncPlan  *plan);

G_END_DECLS
