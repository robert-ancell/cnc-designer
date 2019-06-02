/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <gtk/gtk.h>

#include "cnc-application.h"

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (CncApplicationWindow, cnc_application_window, CNC, APPLICATION_WINDOW, GtkApplicationWindow)

CncApplicationWindow *cnc_application_window_new (CncApplication *application);

G_END_DECLS
