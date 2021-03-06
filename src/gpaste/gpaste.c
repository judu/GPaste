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

#include <gpaste-client.h>

#include <gio/gio.h>
#include <glib/gi18n-lib.h>

#include <stdio.h>
#include <stdlib.h>

static void
show_help (const gchar *caller)
{
    printf (_("Usage:\n"));
    printf (_("  %s [history]: print the history with indexes\n"), caller);
    printf (_("  %s backup-history <name>: backup current history\n"), caller);
    printf (_("  %s switch-history <name>: switch to another history\n"), caller);
    printf (_("  %s delete-history <name>: delete a history\n"), caller);
    printf (_("  %s list-histories: list available histories\n"), caller);
    printf (_("  %s raw-history: print the history without indexes\n"), caller);
    printf (_("  %s zero-history: print the history with NUL as separator\n"), caller);
    printf (_("  %s add <text>: set text to clipboard\n"), caller);
    printf (_("  %s get <number>: get the <number>th item from the history\n"), caller);
    printf (_("  %s select <number>: set the <number>th item from the history to the clipboard\n"), caller);
    printf (_("  %s delete <number>: delete <number>th item of the history\n"), caller);
    printf (_("  %s file <path>: put the content of the file at <path> into the clipboard\n"), caller);
    printf (_("  whatever | %s: set the output of whatever to clipboard\n"), caller);
    printf (_("  %s empty: empty the history\n"), caller);
    printf (_("  %s start: start tracking clipboard changes\n"), caller);
    printf (_("  %s stop: stop tracking clipboard changes\n"), caller);
    printf (_("  %s quit: alias for stop\n"), caller);
    printf (_("  %s daemon-reexec: reexecute the daemon (after upgrading...)\n"), caller);
    printf (_("  %s settings: launch the configuration tool\n"), caller);
#ifdef ENABLE_APPLET
    printf (_("  %s applet: launch the applet\n"), caller);
#endif
    printf (_("  %s version: display the version\n"), caller);
    printf (_("  %s help: display this help\n"), caller);
}

static void
show_version (void)
{
    printf ("%s\n", PACKAGE_STRING);
}

static void
show_history (GPasteClient *client,
              gboolean      raw,
              gboolean      zero,
              GError      **error)
{
    GStrv history = g_paste_client_get_history (client, error);

    if (!*error)
    {
        unsigned int i = 0;

        for (GStrv h = history; *h; ++h)
        {
            if (!raw)
                printf ("%d: ", i++);
            printf ("%s%c", *h, (zero) ? '\0' : '\n');
        }

        g_strfreev (history);
    }
}

static gboolean
is_help (const gchar *option)
{
    return (!g_strcmp0 (option, "help") ||
            !g_strcmp0 (option, "-h") ||
            !g_strcmp0 (option, "--help"));
}

static gboolean
is_version (const gchar *option)
{
    return (!g_strcmp0 (option, "v") ||
            !g_strcmp0 (option, "version") ||
            !g_strcmp0 (option, "-v") ||
            !g_strcmp0 (option, "--version"));
}

static void
failure_exit (GError *error)
{
    g_error ("%s: %s\n", _("Couldn't connect to GPaste daemon"), error->message);
    g_error_free (error);
    exit (EXIT_FAILURE);
}

gint
main (gint argc, gchar *argv[])
{
    bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);

    if (is_help (argv[1]))
    {
        show_help (argv[0]);
        return EXIT_SUCCESS;
    }
    else if (is_version (argv[1]))
    {
        show_version ();
        return EXIT_SUCCESS;
    }

    int status = EXIT_SUCCESS;

    GError *error = NULL;
    GPasteClient *client = g_paste_client_new (&error);

    if (!client)
        failure_exit (error);

    if (!isatty (fileno (stdin)))
    {
        /* We are being piped */
        GString *data = g_string_new ("");
        gchar c;

        while ((c = fgetc (stdin)) != EOF)
            data = g_string_append_c (data, c);

        data->str[data->len - 1] = '\0';

        g_paste_client_add (client, data->str, &error);

        g_string_free (data, TRUE);
    }
    else
    {
        const gchar *arg1, *arg2;
        switch (argc)
        {
        case 1:
            show_history (client, FALSE, FALSE, &error);
            break;
        case 2:
            arg1 = argv[1];
            if (!g_strcmp0 (arg1, "start") ||
                !g_strcmp0 (arg1, "d") ||
                !g_strcmp0 (arg1, "daemon"))
            {
                g_paste_client_track (client, TRUE, &error);
            }
            else if (!g_strcmp0 (arg1, "stop") ||
                     !g_strcmp0 (arg1, "q") ||
                     !g_strcmp0 (arg1, "quit"))
            {
                g_paste_client_track (client, FALSE, &error);
            }
            else if (!g_strcmp0 (arg1, "e") ||
                     !g_strcmp0 (arg1, "empty"))
            {
                g_paste_client_empty (client, &error);
            }
#ifdef ENABLE_APPLET
            else if (!g_strcmp0 (arg1, "applet"))
            {
                g_spawn_command_line_async (PKGLIBEXECDIR "/gpaste-applet", &error);
                if (error)
                {
                    g_error ("%s: %s", _("Couldn't spawn gpaste-applet.\n"), error->message);
                    g_clear_error (&error);
                    status = EXIT_FAILURE;
                }
            }
#endif
            else if (!g_strcmp0 (arg1, "s") ||
                     !g_strcmp0 (arg1, "settings") ||
                     !g_strcmp0 (arg1, "p") ||
                     !g_strcmp0 (arg1, "preferences"))
            {
                execl (PKGLIBEXECDIR "/gpaste-settings", "GPaste-Settings", NULL);
            }
            else if (!g_strcmp0 (arg1, "dr") ||
                     !g_strcmp0 (arg1, "daemon-reexec"))
            {
                g_paste_client_reexecute (client, &error);
                if (error && error->code == G_DBUS_ERROR_NO_REPLY)
                {
                    printf (_("Successfully reexecuted the daemon\n"));
                }
            }
            else if (!g_strcmp0 (arg1, "h") ||
                     !g_strcmp0 (arg1, "history"))
            {
                show_history (client, FALSE, FALSE, &error);
            }
            else if (!g_strcmp0 (arg1, "rh") ||
                     !g_strcmp0 (arg1, "raw-history"))
            {
                show_history (client, TRUE, FALSE, &error);
            }
            else if (!g_strcmp0 (arg1, "zh") ||
                     !g_strcmp0 (arg1, "zero-history"))
            {
                show_history (client, FALSE, TRUE, &error);
            }
            else if (!g_strcmp0 (arg1, "lh") ||
                     !g_strcmp0 (arg1, "list-histories"))
            {
                GStrv histories = g_paste_client_list_histories (client, &error);
                if (!error)
                {
                    for (GStrv h = histories; *h; ++h)
                        printf ("%s\n", *h);
                    g_strfreev (histories);
                }
            }
            else
            {
                show_help (argv[0]);
                status = EXIT_FAILURE;
            }
            break;
        case 3:
            arg1 = argv[1];
            arg2 = argv[2];
            if (!g_strcmp0 (arg1, "bh")||
                !g_strcmp0 (arg1, "backup-history"))
            {
                g_paste_client_backup_history (client, arg2, &error);
            }
            else if (!g_strcmp0 (arg1, "sh") ||
                     !g_strcmp0 (arg1, "switch-history"))
            {
                g_paste_client_switch_history (client, arg2, &error);
            }
            else if (!g_strcmp0 (arg1, "dh") ||
                     !g_strcmp0 (arg1, "delete-history"))
            {
                g_paste_client_delete_history (client, arg2, &error);
            }
            else if (!g_strcmp0 (arg1, "a") ||
                     !g_strcmp0 (arg1, "add"))
            {
                g_paste_client_add (client, arg2, &error);
            }
            else if (!g_strcmp0 (arg1, "g")||
                     !g_strcmp0 (arg1, "get"))
            {
                printf ("%s", g_paste_client_get_element (client, g_ascii_strtoull (arg2, NULL, 0), &error));
            }
            else if (!g_strcmp0 (arg1, "s") ||
                     !g_strcmp0 (arg1, "set") ||
                     !g_strcmp0 (arg1, "select"))
            {
                g_paste_client_select (client, g_ascii_strtoull (arg2, NULL, 0), &error);
            }
            else if (!g_strcmp0 (arg1, "d") ||
                     !g_strcmp0 (arg1, "delete"))
            {
                g_paste_client_delete (client, g_ascii_strtoull (arg2, NULL, 0), &error);
            }
            else if (!g_strcmp0 (arg1, "f") ||
                     !g_strcmp0 (arg1, "file"))
            {
                g_paste_client_add_file (client, arg2, &error);
            }
            else
            {
                show_help (argv[0]);
                status = EXIT_FAILURE;
            }
            break;
        default:
            show_help (argv[0]);
            status = EXIT_FAILURE;
            break;
        }
    }

    g_object_unref (client);

    if (error)
        failure_exit (error);

    return status;
}
