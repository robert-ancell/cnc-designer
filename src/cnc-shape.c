/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <json-glib/json-glib.h>

#include "cnc-shape.h"

struct _CncShape
{
    GObject parent_instance;

    GPtrArray *lines;
};

G_DEFINE_TYPE (CncShape, cnc_shape, G_TYPE_OBJECT)

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
cnc_shape_dispose (GObject *object)
{
    CncShape *self = CNC_SHAPE (object);
    g_clear_pointer (&self->lines, g_ptr_array_unref);
}

static void
cnc_shape_class_init (CncShapeClass *klass)
{
    G_OBJECT_CLASS (klass)->dispose = cnc_shape_dispose;
}

static void
cnc_shape_init (CncShape *self)
{
     self->lines = g_ptr_array_new_with_free_func ((GDestroyNotify) cnc_line_free);
}

CncShape *
cnc_shape_new (void)
{
    return g_object_new (cnc_shape_get_type (), NULL);
}

static gboolean
set_from_json (CncShape *self, JsonNode *node, GError **error)
{
    g_ptr_array_set_size (self->lines, 0);

    // FIXME: checks
    JsonObject *root = json_node_get_object (node);
    JsonArray *lines = json_object_get_array_member (root, "lines");
    for (guint i = 0; i < json_array_get_length (lines); i++) {
        JsonObject *line_object = json_array_get_object_element (lines, i);
        CncLine *line = cnc_shape_add_line (self);
        line->x0 = json_object_get_double_member (line_object, "x0");
        line->y0 = json_object_get_double_member (line_object, "y0");
        line->x1 = json_object_get_double_member (line_object, "x1");
        line->y1 = json_object_get_double_member (line_object, "y1");
    }

    return TRUE;
}

CncShape *
cnc_shape_new_from_json (JsonNode *node)
{
    CncShape *self = cnc_shape_new ();

    g_autoptr(GError) error = NULL;
    set_from_json (self, node, &error);

    return self;
}

JsonNode *
cnc_shape_to_json (CncShape *self)
{
    g_autoptr(JsonBuilder) builder = json_builder_new ();
    json_builder_begin_object (builder);
    json_builder_set_member_name (builder, "lines");
    json_builder_begin_array (builder);
    for (guint i = 0; i < self->lines->len; i++) {
        CncLine *line = g_ptr_array_index (self->lines, i);
        json_builder_begin_object (builder);
        json_builder_set_member_name (builder, "x0");
        json_builder_add_double_value (builder, line->x0);
        json_builder_set_member_name (builder, "y0");
        json_builder_add_double_value (builder, line->y0);
        json_builder_set_member_name (builder, "x1");
        json_builder_add_double_value (builder, line->x1);
        json_builder_set_member_name (builder, "y1");
        json_builder_add_double_value (builder, line->y1);
        json_builder_end_object (builder);
    }
    json_builder_end_array (builder);
    json_builder_end_object (builder);

    return json_builder_get_root (builder);
}

CncLine *
cnc_shape_add_line (CncShape *self)
{
    g_return_val_if_fail (CNC_IS_SHAPE (self), NULL);
    CncLine *line = cnc_line_new ();
    g_ptr_array_add (self->lines, line);
    return line;
}

void
cnc_shape_remove_line (CncShape *self, CncLine *line)
{
    g_return_if_fail (CNC_IS_SHAPE (self));
    g_ptr_array_remove (self->lines, line);
}

GPtrArray *
cnc_shape_get_lines (CncShape *self)
{
    g_return_val_if_fail (CNC_IS_SHAPE (self), NULL);
    return self->lines;
}
