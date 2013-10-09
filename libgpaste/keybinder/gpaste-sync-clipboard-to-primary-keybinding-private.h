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

#ifndef __G_PASTE_SYNC_CLIPBOARD_TO_PRIMARY_KEYBINDING_PRIVATE_H__
#define __G_PASTE_SYNC_CLIPBOARD_TO_PRIMARY_KEYBINDING_PRIVATE_H__

#include "gpaste-keybinding-private.h"

#include <gpaste-sync-clipboard-to-primary-keybinding.h>

G_BEGIN_DECLS

typedef struct _GPasteSyncClipboardToPrimaryKeybindingPrivate GPasteShowHistoryKeybindingPrivate;

struct _GPasteSyncClipboardToPrimaryKeybinding
{
    GPasteKeybinding parent_instance;
};

struct _GPasteSyncClipboardToPrimaryKeybindingClass
{
    GPasteKeybindingClass parent_class;
};

G_END_DECLS

#endif /*__G_PASTE_SYNC_CLIPBOARD_TO_PRIMARY_KEYBINDING_PRIVATE_H__*/
