/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "cnc-application.h"

struct _CncApplication
{
    GtkApplication parent_instance;
};

G_DEFINE_TYPE (CncApplication, cnc_application, GTK_TYPE_APPLICATION)

static void
cnc_application_activate (CncApplication *self)
{
}

static void
cnc_application_class_init (CncApplicationClass *klass)
{
    GApplicationClass *application_class = G_APPLICATION_CLASS (klass);

    application_class->activate = cnc_application_activate;
}

static void
cnc_application_init (CncApplication *self)
{
}

CncApplication *
cnc_application_new (void)
{
    return g_object_new (cnc_application_get_type (), NULL);
}
