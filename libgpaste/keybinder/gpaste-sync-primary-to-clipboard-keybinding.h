/*
 *      This file is part of GPaste.
 *
 *      Copyright 2013 Marc-Antoine Perennou <Marc-Antoine@Perennou.com>
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

#if !defined (__G_PASTE_H_INSIDE__) && !defined (G_PASTE_COMPILATION)
#error "Only <gpaste.h> can be included directly."
#endif

#ifndef __G_PASTE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING_H__
#define __G_PASTE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING_H__

#include <gpaste-clipboards-manager.h>
#include <gpaste-keybinding.h>

G_BEGIN_DECLS

#define G_PASTE_TYPE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING            (g_paste_sync_primary_to_clipboard_keybinding_get_type ())
#define G_PASTE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_PASTE_TYPE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING, GPasteSyncPrimaryToClipboardKeybinding))
#define G_PASTE_IS_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_PASTE_TYPE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING))
#define G_PASTE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), G_PASTE_TYPE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING, GPasteSyncPrimaryToClipboardKeybindingClass))
#define G_PASTE_IS_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), G_PASTE_TYPE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING))
#define G_PASTE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), G_PASTE_TYPE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING, GPasteSyncPrimaryToClipboardKeybindingClass))

typedef struct _GPasteSyncPrimaryToClipboardKeybinding GPasteSyncPrimaryToClipboardKeybinding;
typedef struct _GPasteSyncPrimaryToClipboardKeybindingClass GPasteSyncPrimaryToClipboardKeybindingClass;

G_PASTE_VISIBLE
GType g_paste_sync_primary_to_clipboard_keybinding_get_type (void);

GPasteKeybinding *g_paste_sync_primary_to_clipboard_keybinding_new (GPasteSettings          *settings,
                                                                    GPasteClipboardsManager *gcm);

G_END_DECLS

#endif /*__G_PASTE_SYNC_PRIMARY_TO_CLIPBOARD_KEYBINDING_H__*/
