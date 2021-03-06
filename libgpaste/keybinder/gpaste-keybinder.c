/*
 *      This file is part of GPaste.
 *
 *      Copyright 2011-2013 Marc-Antoine Perennou <Marc-Antoine@Perennou.com>
 *
 *      GPaste is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      GPaste is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with GPaste.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gpaste-keybinder-private.h"

#include <gdk/gdk.h>

struct _GPasteKeybinderPrivate
{
    GSList    *keybindings;
};

G_DEFINE_TYPE_WITH_PRIVATE (GPasteKeybinder, g_paste_keybinder, G_TYPE_OBJECT)

/**
 * g_paste_keybinder_add_keybinding:
 * @self: a #GPasteKeybinder instance
 * @binding; a #GPasteKeybinding instance
 *
 * Add a new keybinding
 *
 * Returns:
 */
G_PASTE_VISIBLE void
g_paste_keybinder_add_keybinding (GPasteKeybinder  *self,
                                  GPasteKeybinding *binding)
{
    g_return_if_fail (G_PASTE_IS_KEYBINDER (self));
    g_return_if_fail (G_PASTE_IS_KEYBINDING (binding));

    GPasteKeybinderPrivate *priv = g_paste_keybinder_get_instance_private (self);

    priv->keybindings = g_slist_prepend (priv->keybindings,
                                         g_object_ref (binding));
}

static void
g_paste_keybinder_activate_keybinding_func (gpointer data,
                                            gpointer user_data G_GNUC_UNUSED)
{
    GPasteKeybinding *keybinding = data;

    if (!g_paste_keybinding_is_active (keybinding))
        g_paste_keybinding_activate (keybinding);
}

/**
 * g_paste_keybinder_activate_all:
 * @self: a #GPasteKeybinder instance
 *
 * Activate all the managed keybindings
 *
 * Returns:
 */
G_PASTE_VISIBLE void
g_paste_keybinder_activate_all (GPasteKeybinder *self)
{
    g_return_if_fail (G_PASTE_IS_KEYBINDER (self));

    GPasteKeybinderPrivate *priv = g_paste_keybinder_get_instance_private (self);

    g_slist_foreach (priv->keybindings,
                     g_paste_keybinder_activate_keybinding_func,
                     NULL);
}

static void
g_paste_keybinder_deactivate_keybinding_func (gpointer data,
                                              gpointer user_data G_GNUC_UNUSED)
{
    GPasteKeybinding *keybinding = data;

    if (g_paste_keybinding_is_active (keybinding))
        g_paste_keybinding_deactivate (keybinding);
}

/**
 * g_paste_keybinder_deactivate_all:
 * @self: a #GPasteKeybinder instance
 *
 * Deactivate all the managed keybindings
 *
 * Returns:
 */
G_PASTE_VISIBLE void
g_paste_keybinder_deactivate_all (GPasteKeybinder *self)
{
    g_return_if_fail (G_PASTE_IS_KEYBINDER (self));

    GPasteKeybinderPrivate *priv = g_paste_keybinder_get_instance_private (self);

    g_slist_foreach (priv->keybindings,
                     g_paste_keybinder_deactivate_keybinding_func,
                     NULL);
}

static GdkFilterReturn
g_paste_keybinder_filter (GdkXEvent *xevent,
                          GdkEvent  *event G_GNUC_UNUSED,
                          gpointer   data)
{
    GPasteKeybinderPrivate *priv = data;

    for (GList *dev = gdk_device_manager_list_devices (gdk_display_get_device_manager (gdk_display_get_default ()),
                                                       GDK_DEVICE_TYPE_MASTER); dev; dev = g_list_next (dev))
    {
        GdkDevice *device = dev->data;

        if (gdk_device_get_source (device) == GDK_SOURCE_KEYBOARD)
            gdk_device_ungrab (device, GDK_CURRENT_TIME);
    }

    gdk_flush ();

    for (GSList *keybinding = priv->keybindings; keybinding; keybinding = g_slist_next (keybinding))
    {
        GPasteKeybinding *real_keybinding = keybinding->data;
        if (g_paste_keybinding_is_active (real_keybinding))
            g_paste_keybinding_notify (real_keybinding, xevent);
    }

    return GDK_FILTER_CONTINUE;
}

static void
g_paste_keybinder_dispose (GObject *object)
{
    GPasteKeybinder *self = G_PASTE_KEYBINDER (object);
    GPasteKeybinderPrivate *priv = g_paste_keybinder_get_instance_private (self);

    if (priv->keybindings)
    {
        g_paste_keybinder_deactivate_all (self);
        g_slist_foreach (priv->keybindings, (GFunc) g_object_unref, NULL);
        priv->keybindings = NULL;
    }

    G_OBJECT_CLASS (g_paste_keybinder_parent_class)->dispose (object);
}

static void
g_paste_keybinder_finalize (GObject *object)
{
    GPasteKeybinderPrivate *priv = g_paste_keybinder_get_instance_private (G_PASTE_KEYBINDER (object));

    gdk_window_remove_filter (gdk_get_default_root_window (),
                              g_paste_keybinder_filter,
                              priv);

    G_OBJECT_CLASS (g_paste_keybinder_parent_class)->finalize (object);
}

static void
g_paste_keybinder_class_init (GPasteKeybinderClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->dispose = g_paste_keybinder_dispose;
    object_class->finalize = g_paste_keybinder_finalize;
}

static void
g_paste_keybinder_init (GPasteKeybinder *self)
{
    GPasteKeybinderPrivate *priv = g_paste_keybinder_get_instance_private (self);

    priv->keybindings = NULL;

    gdk_window_add_filter (gdk_get_default_root_window (),
                           g_paste_keybinder_filter,
                           priv);
}

/**
 * g_paste_keybinder_new:
 *
 * Create a new instance of #GPasteKeybinder
 *
 * Returns: a newly allocated #GPasteKeybinder
 *          free it with g_object_unref
 */
G_PASTE_VISIBLE GPasteKeybinder *
g_paste_keybinder_new (void)
{
    return g_object_new (G_PASTE_TYPE_KEYBINDER, NULL);
}
