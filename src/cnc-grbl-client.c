/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "cnc-grbl-client.h"

struct _CncGrblClient
{
    GObject parent_instance;

    GCancellable *cancellable;
    GFile *file;
    GFileIOStream *stream;

    gchar *version;
};

G_DEFINE_TYPE (CncGrblClient, cnc_grbl_client, G_TYPE_OBJECT)

static void
cnc_grbl_client_dispose (GObject *object)
{
    CncGrblClient *self = CNC_GRBL_CLIENT (object);
    g_cancellable_cancel (self->cancellable);
    g_clear_object (&self->cancellable);
    g_clear_object (&self->file);
    g_clear_pointer (&self->version, g_free);
}

static void
cnc_grbl_client_class_init (CncGrblClientClass *klass)
{
    G_OBJECT_CLASS (klass)->dispose = cnc_grbl_client_dispose;
}

static void
cnc_grbl_client_init (CncGrblClient *self)
{
    self->cancellable = g_cancellable_new ();
}

CncGrblClient *
cnc_grbl_client_new (const gchar *path)
{
    CncGrblClient *self = g_object_new (cnc_grbl_client_get_type (), NULL);
    self->file = g_file_new_for_path (path);
    return self;
}

static void read_cb (GObject *object, GAsyncResult *result, gpointer user_data);

static void
read_from_controller (CncGrblClient *self)
{
    GInputStream *input_stream = g_io_stream_get_input_stream (G_IO_STREAM (self->stream));
    g_input_stream_read_bytes_async (input_stream, 1024, G_PRIORITY_DEFAULT, self->cancellable, read_cb, self);
}

static void
read_cb (GObject *object, GAsyncResult *result, gpointer user_data)
{
    CncGrblClient *self = user_data;

    g_autoptr(GError) error = NULL;
    g_autoptr(GBytes) data = g_input_stream_read_bytes_finish (G_INPUT_STREAM (object), result, &error);
    if (data == NULL) {
        if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
            return;
    }

    // FIXME: Parse responses

    read_from_controller (self);
}

gboolean
cnc_grbl_client_connect (CncGrblClient *self, GCancellable *cancellable, GError **error)
{
    g_return_val_if_fail (CNC_IS_GRBL_CLIENT (self), FALSE);
    g_return_val_if_fail (self->stream == NULL, FALSE);

    self->stream = g_file_open_readwrite (self->file, cancellable, error);
    if (self->stream == NULL)
        return FALSE;

    read_from_controller (self);

    return TRUE;
}

const gchar *
cnc_grbl_client_get_version (CncGrblClient *self)
{
    g_return_val_if_fail (CNC_IS_GRBL_CLIENT (self), NULL);
    return self->version;
}

void
cnc_grbl_client_send_command (CncGrblClient *self, const gchar *command)
{
    g_return_if_fail (CNC_IS_GRBL_CLIENT (self));
    g_return_if_fail (command != NULL);

    GOutputStream *output_stream = g_io_stream_get_output_stream (G_IO_STREAM (self->stream));
    g_autoptr(GError) error = NULL;
    g_autoptr(GBytes) data = g_bytes_new_static (command, strlen (command));
    g_output_stream_write_bytes (G_OUTPUT_STREAM (output_stream), data, self->cancellable, &error);
}
