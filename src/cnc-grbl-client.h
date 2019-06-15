/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <gio/gio.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE (CncGrblClient, cnc_grbl_client, CNC, GRBL_CLIENT, GObject)

CncGrblClient *cnc_grbl_client_new          (const gchar *path);

gboolean       cnc_grbl_client_connect      (CncGrblClient *client, GCancellable *cancellable, GError **error);

const gchar   *cnc_grbl_client_get_version  (CncGrblClient *self);

void           cnc_grbl_client_send_command (CncGrblClient *self, const gchar *command);

G_END_DECLS
