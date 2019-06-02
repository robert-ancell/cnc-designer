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
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
    gtk_widget_class_set_template_from_resource (widget_class, "/com/example/CncDesigner/cnc-application-window.ui");

    //gtk_widget_class_bind_template_child (widget_class, CncApplicationWindow, ?);
}

static void
cnc_application_window_init (CncApplicationWindow *self)
{
    gtk_widget_init_template (GTK_WIDGET (self));
}

CncApplicationWindow *
cnc_application_window_new (CncApplication *application)
{
    return g_object_new (cnc_application_window_get_type (),
                         "application", application,
                         NULL);
}