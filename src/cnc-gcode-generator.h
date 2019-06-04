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

G_DECLARE_FINAL_TYPE (CncGcodeGenerator, cnc_gcode_generator, CNC, GCODE_GENERATOR, GObject)

typedef enum
{
    CNC_DISTANCE_MODE_ABSOLUTE = 0,
    CNC_DISTANCE_MODE_RELATIVE
} CncDistanceMode;

typedef enum
{
    CNC_SPINDLE_DIRECTION_CLOCKWISE = 0,
    CNC_SPINDLE_DIRECTION_COUNTER_CLOCKWISE
} CncSpindleDirection;

CncGcodeGenerator *cnc_gcode_generator_new                (void);

void               cnc_gcode_generator_add_comment        (CncGcodeGenerator *generator, const gchar *comment);

void               cnc_gcode_generator_add_distance_mode  (CncGcodeGenerator *generator, CncDistanceMode mode);

void               cnc_gcode_generator_add_spindle_on     (CncGcodeGenerator *generator, CncSpindleDirection direction, gint64 speed);

void               cnc_gcode_generator_add_rapid_move_xyz (CncGcodeGenerator *generator, gint64 x, gint64 y, gint64 z);

void               cnc_gcode_generator_add_rapid_move_xy  (CncGcodeGenerator *generator, gint64 x, gint64 y);

void               cnc_gcode_generator_add_rapid_move_z   (CncGcodeGenerator *generator, gint64 z);

void               cnc_gcode_generator_add_move_xyz       (CncGcodeGenerator *generator, gint64 x, gint64 y, gint64 z);

void               cnc_gcode_generator_add_move_xy        (CncGcodeGenerator *generator, gint64 x, gint64 y);

void               cnc_gcode_generator_add_move_z         (CncGcodeGenerator *generator, gint64 z);

gchar             *cnc_gcode_generator_to_data            (CncGcodeGenerator *generator);

G_END_DECLS
