/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "cnc-gcode-generator.h"

struct _CncGcodeGenerator
{
    GObject parent_instance;

    GString *data;
};

G_DEFINE_TYPE (CncGcodeGenerator, cnc_gcode_generator, G_TYPE_OBJECT)

static void
free_data (GString *string)
{
    g_string_free (string, TRUE);
}

static void
cnc_gcode_generator_dispose (GObject *object)
{
    CncGcodeGenerator *self = CNC_GCODE_GENERATOR (object);
    g_clear_pointer (&self->data, free_data);
}

static void
cnc_gcode_generator_class_init (CncGcodeGeneratorClass *klass)
{
    G_OBJECT_CLASS (klass)->dispose = cnc_gcode_generator_dispose;
}

static void
cnc_gcode_generator_init (CncGcodeGenerator *self)
{
     self->data = g_string_new ("");
}

CncGcodeGenerator *
cnc_gcode_generator_new (void)
{
    return g_object_new (cnc_gcode_generator_get_type (), NULL);
}

static void
add_value (CncGcodeGenerator *self, const gchar *address, gint64 value)
{
    g_string_append (self->data, " ");
    g_string_append (self->data, address);
    g_string_append_printf (self->data, "%" G_GINT64_FORMAT, value / 1000);
    guint fraction = value % 1000;
    if (fraction == 0)
        return;
    guint d = 100;
    while (TRUE) {
        guint f = fraction / d;
        if (f == 0)
            return;
        g_string_append_printf (self->data, "%u", f);
        fraction -= f * d;
        if (d == 1)
            return;
        d /= 10;
    }
}

static void
end_line (CncGcodeGenerator *self)
{
    g_string_append (self->data, "\n");
}

void
cnc_gcode_generator_add_comment (CncGcodeGenerator *self, const gchar *comment)
{
    // FIXME: escape!?
    g_string_append_printf (self->data, "(%s)", comment);
    end_line (self);
}

void
cnc_gcode_generator_add_distance_mode (CncGcodeGenerator *self, CncDistanceMode mode)
{
    if (mode == CNC_DISTANCE_MODE_RELATIVE)
        g_string_append (self->data, "G91");
    else
        g_string_append (self->data, "G90");
    end_line (self);
}

void
cnc_gcode_generator_add_units (CncGcodeGenerator *self, CncUnits units)
{
    if (units == CNC_UNITS_INCH)
        g_string_append (self->data, "G20");
    else
        g_string_append (self->data, "G21");
    end_line (self);
}

void
cnc_gcode_generator_add_spindle_on (CncGcodeGenerator *self, CncSpindleDirection direction, gint64 speed)
{
    if (direction == CNC_SPINDLE_DIRECTION_COUNTER_CLOCKWISE)
        g_string_append (self->data, "M04");
    else
        g_string_append (self->data, "M03");
    if (speed != 0)
        add_value (self, "S", speed);
    end_line (self);
}

void
cnc_gcode_generator_add_spindle_stop (CncGcodeGenerator *self)
{
    g_string_append (self->data, "M05");
    end_line (self);
}

void
cnc_gcode_generator_add_rapid_move_xyz (CncGcodeGenerator *self, gint64 x, gint64 y, gint64 z)
{
    g_return_if_fail (CNC_IS_GCODE_GENERATOR (self));
    g_string_append (self->data, "G0");
    add_value (self, "X", x);
    add_value (self, "Y", y);
    add_value (self, "Z", z);
    end_line (self);
}

void
cnc_gcode_generator_add_rapid_move_xy (CncGcodeGenerator *self, gint64 x, gint64 y)
{
    g_return_if_fail (CNC_IS_GCODE_GENERATOR (self));
    g_string_append (self->data, "G0");
    add_value (self, "X", x);
    add_value (self, "Y", y);
    end_line (self);
}

void
cnc_gcode_generator_add_rapid_move_z (CncGcodeGenerator *self, gint64 z)
{
    g_return_if_fail (CNC_IS_GCODE_GENERATOR (self));
    g_string_append (self->data, "G0");
    add_value (self, "Z", z);
    end_line (self);
}
void
cnc_gcode_generator_add_move_xyz (CncGcodeGenerator *self, gint64 x, gint64 y, gint64 z)
{
    g_return_if_fail (CNC_IS_GCODE_GENERATOR (self));
    g_string_append (self->data, "G1");
    add_value (self, "X", x);
    add_value (self, "Y", y);
    add_value (self, "Z", z);
    end_line (self);
}

void
cnc_gcode_generator_add_move_xy (CncGcodeGenerator *self, gint64 x, gint64 y)
{
    g_return_if_fail (CNC_IS_GCODE_GENERATOR (self));
    g_string_append (self->data, "G1");
    add_value (self, "X", x);
    add_value (self, "Y", y);
    end_line (self);
}

void
cnc_gcode_generator_add_move_z (CncGcodeGenerator *self, gint64 z)
{
    g_return_if_fail (CNC_IS_GCODE_GENERATOR (self));
    g_string_append (self->data, "G1");
    add_value (self, "Z", z);
    end_line (self);
}

void
cnc_gcode_generator_add_end_of_program (CncGcodeGenerator *self)
{
    g_return_if_fail (CNC_IS_GCODE_GENERATOR (self));
    g_string_append (self->data, "M30");
    end_line (self);
}

gchar *
cnc_gcode_generator_to_data (CncGcodeGenerator *self)
{
    g_return_val_if_fail (CNC_IS_GCODE_GENERATOR (self), NULL);
    return g_strdup (self->data->str);
}
