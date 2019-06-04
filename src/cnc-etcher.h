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

#include "cnc-shape.h"
#include "cnc-gcode-generator.h"

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (CncEtcher, cnc_etcher, CNC, ETCHER, GObject)

CncEtcher *cnc_etcher_new            (void);

void       cnc_etcher_set_depth      (CncEtcher *etcher, gint64 depth);

void       cnc_etcher_add_shape      (CncEtcher *etcher, CncShape *shape);

void       cnc_etcher_generate_gcode (CncEtcher *etcher, CncGcodeGenerator *generator);

G_END_DECLS
