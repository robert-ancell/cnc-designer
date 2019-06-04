/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "cnc-application-window.h"
#include "cnc-plan.h"
#include "cnc-plan-view.h"

struct _CncApplicationWindow
{
    GtkApplicationWindow parent_instance;

    CncPlanView *plan_view;

    CncPlan *plan;
};

G_DEFINE_TYPE (CncApplicationWindow, cnc_application_window, GTK_TYPE_APPLICATION_WINDOW)

static void
cnc_application_window_class_init (CncApplicationWindowClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
    gtk_widget_class_set_template_from_resource (widget_class, "/com/example/CncDesigner/cnc-application-window.ui");

    gtk_widget_class_bind_template_child (widget_class, CncApplicationWindow, plan_view);
}

static void
cnc_application_window_init (CncApplicationWindow *self)
{
    cnc_plan_view_get_type ();
    gtk_widget_init_template (GTK_WIDGET (self));

    g_autoptr(GError) error = NULL;
    g_autofree gchar *save_data = NULL;
    if (g_file_get_contents ("cnc-autosave.json", &save_data, NULL, &error)) {
        g_autoptr(JsonParser) parser = json_parser_new ();
        if (json_parser_load_from_data (parser, save_data, -1, &error))
            self->plan = cnc_plan_new_from_json (json_parser_get_root (parser));
    }
    if (self->plan == NULL) {
        self->plan = cnc_plan_new ();
        cnc_plan_add_layer (self->plan);
    }
    cnc_plan_view_set_plan (self->plan_view, self->plan);
}

CncApplicationWindow *
cnc_application_window_new (CncApplication *application)
{
    return g_object_new (cnc_application_window_get_type (),
                         "application", application,
                         NULL);
}
