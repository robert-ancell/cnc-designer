/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "cnc-application-window.h"

struct _CncApplicationWindow
{
    GtkApplicationWindow parent_instance;
};

G_DEFINE_TYPE (CncApplicationWindow, cnc_application_window, GTK_TYPE_APPLICATION_WINDOW)

static void
cnc_application_window_class_init (CncApplicationWindowClass *klass)
{
}

static void
cnc_application_window_init (CncApplicationWindow *self)
{
}

CncApplicationWindow *
cnc_application_window_new (CncApplication *application)
{
    return g_object_new (cnc_application_window_get_type (),
                         "application", application,
                         NULL);
}
