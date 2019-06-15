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
};

G_DEFINE_TYPE (CncGrblClient, cnc_grbl_client, G_TYPE_OBJECT)

static void
cnc_grbl_client_dispose (GObject *object)
{
    CncGrblClient *self = CNC_GRBL_CLIENT (object);
}

static void
cnc_grbl_client_class_init (CncGrblClientClass *klass)
{
    G_OBJECT_CLASS (klass)->dispose = cnc_grbl_client_dispose;
}

static void
cnc_grbl_client_init (CncGrblClient *self)
{
}

CncGrblClient *
cnc_grbl_client_new (void)
{
    return g_object_new (cnc_grbl_client_get_type (), NULL);
}
