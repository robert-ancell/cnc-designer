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

    GPtrArray *layers;
};

G_DEFINE_TYPE (CncPlan, cnc_plan, G_TYPE_OBJECT)

static void
cnc_plan_dispose (GObject *object)
{
    CncPlan *self = CNC_PLAN (object);
    g_clear_pointer (&self->layers, g_ptr_array_unref);
}

static void
cnc_plan_class_init (CncPlanClass *klass)
{
    G_OBJECT_CLASS (klass)->dispose = cnc_plan_dispose;
}

static void
cnc_plan_init (CncPlan *self)
{
     self->layers = g_ptr_array_new_with_free_func (g_object_unref);
}

CncPlan *
cnc_plan_new (void)
{
    return g_object_new (cnc_plan_get_type (), NULL);
}

static gboolean
set_from_json (CncPlan *self, JsonNode *node, GError **error)
{
    g_ptr_array_set_size (self->layers, 0);

    // FIXME: checks
    JsonObject *root = json_node_get_object (node);
    if (json_object_has_member (root, "layers")) {
        JsonArray *layers = json_object_get_array_member (root, "layers");
        for (guint i = 0; i < json_array_get_length (layers); i++) {
            CncLayer *layer = cnc_layer_new_from_json (json_array_get_element (layers, i));
            g_ptr_array_add (self->layers, layer);
        }
    }

    return TRUE;
}

CncPlan *
cnc_plan_new_from_json (JsonNode *node)
{
    CncPlan *plan = cnc_plan_new ();

    g_autoptr(GError) error = NULL;
    set_from_json (plan, node, &error);

    return plan;
}

JsonNode *
cnc_plan_to_json (CncPlan *self)
{
    g_autoptr(JsonBuilder) builder = json_builder_new ();

    json_builder_begin_object (builder);
    json_builder_set_member_name (builder, "layers");
    json_builder_begin_array (builder);
    for (guint i = 0; i < self->layers->len; i++) {
        CncLayer *layer = g_ptr_array_index (self->layers, i);
        json_builder_add_value (builder, cnc_layer_to_json (layer));
    }
    json_builder_end_array (builder);
    json_builder_end_object (builder);

    return json_builder_get_root (builder);
}

CncLayer *
cnc_plan_add_layer (CncPlan *self)
{
    g_return_val_if_fail (CNC_IS_PLAN (self), NULL);
    CncLayer *layer = cnc_layer_new ();
    g_ptr_array_add (self->layers, layer);
    return layer;
}

GPtrArray *
cnc_plan_get_layers (CncPlan *self)
{
    g_return_val_if_fail (CNC_IS_PLAN (self), NULL);
    return self->layers;
}
