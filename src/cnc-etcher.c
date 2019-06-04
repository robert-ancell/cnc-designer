/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "cnc-etcher.h"

struct _CncEtcher
{
    GObject parent_instance;

    gint64 safe_height;
    gint64 depth;
    GPtrArray *shapes;
};

G_DEFINE_TYPE (CncEtcher, cnc_etcher, G_TYPE_OBJECT)

static void
cnc_etcher_dispose (GObject *object)
{
    CncEtcher *self = CNC_ETCHER (object);
    g_clear_pointer (&self->shapes, g_ptr_array_unref);
}

static void
cnc_etcher_class_init (CncEtcherClass *klass)
{
    G_OBJECT_CLASS (klass)->dispose = cnc_etcher_dispose;
}

static void
cnc_etcher_init (CncEtcher *self)
{
    self->safe_height = -2000; // 2mm
    self->depth = 1000; // 1mm
    self->shapes = g_ptr_array_new_with_free_func (g_object_unref);
}

CncEtcher *
cnc_etcher_new (void)
{
    return g_object_new (cnc_etcher_get_type (), NULL);
}

void
cnc_etcher_set_depth (CncEtcher *self, gint64 depth)
{
    g_return_if_fail (CNC_IS_ETCHER (self));
    self->depth = depth;
}

void
cnc_etcher_add_shape (CncEtcher *self, CncShape *shape)
{
    g_return_if_fail (CNC_IS_ETCHER (self));
    g_ptr_array_add (self->shapes, g_object_ref (shape));
}

void
cnc_etcher_generate_gcode (CncEtcher *self, CncGcodeGenerator *generator)
{
    CncLine *last_line = NULL;
    for (guint i = 0; i < self->shapes->len; i++) {
        CncShape *shape = g_ptr_array_index (self->shapes, i);
        GPtrArray *lines = cnc_shape_get_lines (shape);
        for (guint j = 0; j < lines->len; j++) {
            CncLine *line = g_ptr_array_index (lines, j);
            if (last_line == NULL || last_line->x1 != line->x0 || last_line->y1 != line->y0) {
                cnc_gcode_generator_add_rapid_move_z (generator, self->safe_height);
                cnc_gcode_generator_add_rapid_move_xy (generator, line->x0 * 1000, line->y0 * 1000); // FIXME: Convert lines to using nanometers
                cnc_gcode_generator_add_move_z (generator, self->depth);
            }
            else
                cnc_gcode_generator_add_move_xy (generator, line->x0 * 1000, line->y0 * 1000);
            cnc_gcode_generator_add_move_xy (generator, line->x1 * 1000, line->y1 * 1000);
            last_line = line;
        }
    }
}
