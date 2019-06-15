/*
 * Copyright (C) 2019 Robert Ancell.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

#include "cnc-grbl-client.h"

struct _CncGrblClient
{
    GObject parent_instance;

    GCancellable *cancellable;
    gchar *path;
    GIOChannel *channel;
    GSource *read_source;

    gchar *version;
};

G_DEFINE_TYPE (CncGrblClient, cnc_grbl_client, G_TYPE_OBJECT)

static void
cnc_grbl_client_dispose (GObject *object)
{
    CncGrblClient *self = CNC_GRBL_CLIENT (object);
    g_cancellable_cancel (self->cancellable);
    g_clear_object (&self->cancellable);
    g_clear_pointer (&self->path, g_free);
    g_clear_pointer (&self->channel, g_io_channel_unref);
    g_clear_pointer (&self->read_source, g_source_unref);
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
    self->path = g_strdup (path);
    return self;
}

static gboolean
read_cb (GIOChannel *channel, GIOCondition condition, gpointer user_data)
{
    CncGrblClient *self = user_data;

    gchar data[1024];
    gsize n_read;
    g_autoptr(GError) error = NULL;
    GIOStatus status = g_io_channel_read_chars (self->channel, data, 1024, &n_read, &error);
    if (status == G_IO_STATUS_ERROR) {
        g_warning ("Failed to read from GRBL controller: %s", error->message);
        return G_SOURCE_REMOVE;
    }

    if (status == G_IO_STATUS_EOF) {
        g_warning ("GRBL controller closed connection");
        return G_SOURCE_REMOVE;
    }

    if (status != G_IO_STATUS_NORMAL)
        return G_SOURCE_CONTINUE;

    // FIXME: Parse responses
    g_printerr ("read '");
    for (gsize i = 0; i < n_read; i++) {
        gchar c = data[i];
        if (c == '\n')
            g_printerr ("\\n");
        else if (c == '\r')
            g_printerr ("\\r");
        else if (c == '\\')
            g_printerr ("\\");
        else
            g_printerr ("%c", c);
    }
    g_printerr ("'\n");

    return G_SOURCE_CONTINUE;
}

gboolean
cnc_grbl_client_connect (CncGrblClient *self, GCancellable *cancellable, GError **error)
{
    g_return_val_if_fail (CNC_IS_GRBL_CLIENT (self), FALSE);
    g_return_val_if_fail (self->channel == NULL, FALSE);

    int fd = open (self->path, O_RDWR | O_NOCTTY | O_NDELAY);

    /* Configure port */
    struct termios termios;
    tcgetattr (fd, &termios);
    cfsetispeed (&termios, B115200);
    cfsetospeed (&termios, B115200);
    termios.c_cflag &= ~CSIZE;
    termios.c_cflag |= CS8;
    termios.c_cflag |= CLOCAL | CREAD;
    termios.c_cflag &= ~(PARENB | PARODD);
    termios.c_cflag &= ~CSTOPB;
    termios.c_iflag = IGNBRK;
    termios.c_lflag = ECHONL | NOFLSH;
    termios.c_oflag = 0;
    tcsetattr (fd, TCSANOW, &termios);
    tcflush (fd, TCOFLUSH);
    tcflush (fd, TCIFLUSH);

    self->channel = g_io_channel_unix_new (fd);
    g_io_channel_set_close_on_unref (self->channel, TRUE);
    self->read_source = g_io_create_watch (self->channel, G_IO_IN);
    g_source_set_callback (self->read_source, G_SOURCE_FUNC (read_cb), self, NULL);
    GMainContext *context = g_main_context_get_thread_default ();
    if (context == NULL)
        context = g_main_context_default ();
    g_source_attach (self->read_source, context);

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
    g_return_if_fail (self->channel != NULL);

    g_autoptr(GError) error = NULL;
    gsize command_length = strlen (command);
    gsize n_written;
    GIOStatus status = g_io_channel_write_chars (self->channel, command, command_length, &n_written, &error); // FIXME: handle short writes
    if (status == G_IO_STATUS_ERROR)
        g_warning ("Failed to write to GRBL controller: %s", error->message);
    g_assert_cmpint (n_written, ==, command_length);
}
