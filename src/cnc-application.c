/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "cnc-application.h"
#include "cnc-application-window.h"

struct _CncApplication
{
    GtkApplication parent_instance;

    CncApplicationWindow *window;
};

G_DEFINE_TYPE (CncApplication, cnc_application, GTK_TYPE_APPLICATION)

static void
cnc_application_startup (GApplication *application)
{
    CncApplication *self = CNC_APPLICATION (application);

    G_APPLICATION_CLASS (cnc_application_parent_class)->startup (application);

    self->window = cnc_application_window_new (self);
}

static void
cnc_application_activate (GApplication *application)
{
    CncApplication *self = CNC_APPLICATION (application);
    gtk_window_present (GTK_WINDOW (self->window));
}

static void
cnc_application_class_init (CncApplicationClass *klass)
{
    G_APPLICATION_CLASS (klass)->startup = cnc_application_startup;
    G_APPLICATION_CLASS (klass)->activate = cnc_application_activate;
}

static void
cnc_application_init (CncApplication *self G_GNUC_UNUSED)
{
}

CncApplication *
cnc_application_new (void)
{
    return g_object_new (cnc_application_get_type (), NULL);
}
