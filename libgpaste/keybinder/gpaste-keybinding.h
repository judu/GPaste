/*
 *      This file is part of GPaste.
 *
 *      Copyright 2012-2013 Marc-Antoine Perennou <Marc-Antoine@Perennou.com>
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

#ifndef __G_PASTE_KEYBINDING_H__
#define __G_PASTE_KEYBINDING_H__

#ifdef G_PASTE_COMPILATION
#include "config.h"
#endif

#include <gpaste-settings.h>

#include <gdk/gdk.h>
#ifdef GDK_WINDOWING_WAYLAND
#  include <gdk/gdkwayland.h>
#endif
#ifdef GDK_WINDOWING_X11
#  include <gdk/gdkx.h>
#  include <X11/extensions/XInput2.h>
#endif

G_BEGIN_DECLS

#define G_PASTE_TYPE_KEYBINDING            (g_paste_keybinding_get_type ())
#define G_PASTE_KEYBINDING(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_PASTE_TYPE_KEYBINDING, GPasteKeybinding))
#define G_PASTE_IS_KEYBINDING(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_PASTE_TYPE_KEYBINDING))
#define G_PASTE_KEYBINDING_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), G_PASTE_TYPE_KEYBINDING, GPasteKeybindingClass))
#define G_PASTE_IS_KEYBINDING_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), G_PASTE_TYPE_KEYBINDING))
#define G_PASTE_KEYBINDING_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), G_PASTE_TYPE_KEYBINDING, GPasteKeybindingClass))

typedef struct _GPasteKeybinding GPasteKeybinding;
typedef struct _GPasteKeybindingClass GPasteKeybindingClass;
typedef const gchar *(*GPasteKeybindingGetter) (const GPasteSettings *settings);
typedef void  (*GPasteKeybindingFunc) (GPasteKeybinding *self, /* FIXME: g-i */
                                       gpointer          data);

#ifdef G_PASTE_COMPILATION
G_PASTE_VISIBLE
#endif
GType g_paste_keybinding_get_type (void);

void            g_paste_keybinding_activate      (GPasteKeybinding   *self);
void            g_paste_keybinding_deactivate    (GPasteKeybinding   *self);
gboolean        g_paste_keybinding_is_active     (GPasteKeybinding   *self);
void            g_paste_keybinding_notify        (GPasteKeybinding   *self,
                                                  const ClutterEvent *event);

GPasteKeybinding *g_paste_keybinding_new (GPasteSettings        *settings,
                                          const gchar           *dconf_key,
                                          GPasteKeybindingGetter getter,
                                          GPasteKeybindingFunc   callback,
                                          gpointer               user_data);

G_END_DECLS

#endif /*__G_PASTE_KEYBINDING_H__*/
