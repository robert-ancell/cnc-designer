/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <json-glib/json-glib.h>

#include "cnc-layer.h"

struct _CncLayer
{
    GObject parent_instance;

    gchar *name;
    GPtrArray *shapes;
};

G_DEFINE_TYPE (CncLayer, cnc_layer, G_TYPE_OBJECT)

static void
cnc_layer_dispose (GObject *object)
{
    CncLayer *self = CNC_LAYER (object);
    g_clear_pointer (&self->name, g_free);
    g_clear_pointer (&self->shapes, g_ptr_array_unref);
}

static void
cnc_layer_class_init (CncLayerClass *klass G_GNUC_UNUSED)
{
    G_OBJECT_CLASS (klass)->dispose = cnc_layer_dispose;
}

static void
cnc_layer_init (CncLayer *self)
{
     self->shapes = g_ptr_array_new_with_free_func (g_object_unref);
}

CncLayer *
cnc_layer_new (void)
{
    return g_object_new (cnc_layer_get_type (), NULL);
}

static gboolean
set_from_json (CncLayer *self, JsonNode *node, GError **error)
{
    g_ptr_array_set_size (self->shapes, 0);

    // FIXME: checks
    JsonObject *root = json_node_get_object (node);
    if (json_object_has_member (root, "name"))
        cnc_layer_set_name (self, json_object_get_string_member (root, "name"));
    if (json_object_has_member (root, "shapes")) {
        JsonArray *shapes = json_object_get_array_member (root, "shapes");
        for (guint i = 0; i < json_array_get_length (shapes); i++) {
            CncShape *shape = cnc_shape_new_from_json (json_array_get_element (shapes, i));
            g_ptr_array_add (self->shapes, shape);
        }
    }

    return TRUE;
}

CncLayer *
cnc_layer_new_from_json (JsonNode *node)
{
    CncLayer *self = cnc_layer_new ();

    g_autoptr(GError) error = NULL;
    set_from_json (self, node, &error);

    return self;
}

JsonNode *
cnc_layer_to_json (CncLayer *self)
{
    g_autoptr(JsonBuilder) builder = json_builder_new ();
    json_builder_begin_object (builder);
    if (self->name != NULL) {
        json_builder_set_member_name (builder, "name");
        json_builder_add_string_value (builder, self->name);
    }
    json_builder_set_member_name (builder, "shapes");
    json_builder_begin_array (builder);
    for (guint i = 0; i < self->shapes->len; i++) {
        CncShape *shape = g_ptr_array_index (self->shapes, i);
        json_builder_add_value (builder, cnc_shape_to_json (shape));
    }
    json_builder_end_array (builder);
    json_builder_end_object (builder);

    return json_builder_get_root (builder);
}

void
cnc_layer_set_name (CncLayer *self, const gchar *name)
{
    g_return_if_fail (CNC_IS_LAYER (self));
    g_free (self->name);
    self->name = g_strdup (name);
}

const gchar *
cnc_layer_get_name (CncLayer *self)
{
    g_return_val_if_fail (CNC_IS_LAYER (self), NULL);
    return self->name;
}

CncShape *
cnc_layer_add_shape (CncLayer *self)
{
    g_return_val_if_fail (CNC_IS_LAYER (self), NULL);
    CncShape *shape = cnc_shape_new ();
    g_ptr_array_add (self->shapes, shape);
    return shape;
}

GPtrArray *
cnc_layer_get_shapes (CncLayer *self)
{
    g_return_val_if_fail (CNC_IS_LAYER (self), NULL);
    return self->shapes;
}
