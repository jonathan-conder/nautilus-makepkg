/* 
 * Copyright (C) 2010 Jonathan Conder <j@skurvy.no-ip.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib/gi18n-lib.h>
#include <gconf/gconf-client.h>
#define GNOME_DESKTOP_USE_UNSTABLE_API
#include <libgnome/gnome-desktop-utils.h>
#include <libnautilus-extension/nautilus-menu-provider.h>
#include "nautilus-makepkg.h"

#define NAUTILUS_MAKEPKG_CONF_ENABLED "/apps/nautilus-makepkg/enabled"
#define NAUTILUS_MAKEPKG_CONF_OPTIONS "/apps/nautilus-makepkg/options"

struct _NautilusMakepkgPrivate {
	GConfClient *conf;
};

static void nautilus_makepkg_init (NautilusMakepkg *self) {
	NautilusMakepkgPrivate *priv = self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, NAUTILUS_TYPE_MAKEPKG, NautilusMakepkgPrivate);
	priv->conf = gconf_client_get_default ();
}

static void nautilus_makepkg_finalize (GObject *obj) {
	NautilusMakepkgPrivate *priv = NAUTILUS_MAKEPKG (obj)->priv;
	g_object_unref (priv->conf);
}

typedef struct _MenuItemActivateData {
	NautilusMakepkg *self;
	GdkScreen *screen;
	gchar *path;
} MenuItemActivateData;

static void menu_item_activate_data_free (gpointer user_data, GClosure *closure) {
	MenuItemActivateData *data = (MenuItemActivateData *) user_data;
	g_object_unref (data->self);
	g_object_unref (data->screen);
	g_free (data->path);
	g_free (data);
}

static void nautilus_makepkg_menu_item_activate_cb (NautilusMenuItem *item, gpointer user_data) {
	gchar *options, *command, **argv;
	gint argc;
	GError *error = NULL;

	MenuItemActivateData *data = (MenuItemActivateData *) user_data;
	NautilusMakepkgPrivate *priv = data->self->priv;

	options = gconf_client_get_string (priv->conf, NAUTILUS_MAKEPKG_CONF_OPTIONS, &error);
	if (options == NULL) {
		g_warning ("Unable to retrieve key " NAUTILUS_MAKEPKG_CONF_OPTIONS ": %s\n", error->message);
		g_error_free (error);
		return;
	}

	command = g_strdup_printf ("%s %s", NAUTILUS_MAKEPKG_COMMAND, options);
	g_free (options);

	if (!g_shell_parse_argv (command, &argc, &argv, &error)) {
		g_warning ("Unable to understand options: %s\n", error->message);
		g_error_free (error);
		g_free (command);
		return;
	}

	gnome_desktop_prepend_terminal_to_vector (&argc, &argv);
	g_free (command);

	if (!gdk_spawn_on_screen (data->screen, data->path, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, &error)) {
		g_warning ("Unable to run makepkg: %s\n", error->message);
		g_error_free (error);
		g_strfreev (argv);
		return;
	}

	g_strfreev (argv);
	return;
}

static NautilusMenuItem *nautilus_makepkg_menu_item_new (NautilusMakepkg *self, GtkWidget *window, const gchar *path) {
	NautilusMenuItem *item = nautilus_menu_item_new ("makepkg", _("_Make Package"), _("Build a package from this PKGBUILD"), "gnome-terminal");

	MenuItemActivateData *data = g_new0 (MenuItemActivateData, 1);
	data->self = g_object_ref (self);
	data->screen = g_object_ref (gtk_widget_get_screen (window));
	data->path = g_strdup (path);

	g_signal_connect_data (item, "activate", G_CALLBACK (nautilus_makepkg_menu_item_activate_cb), data, menu_item_activate_data_free, 0);

	return item;
}

static GList *nautilus_makepkg_get_file_items (NautilusMenuProvider *provider, GtkWidget *window, GList *files) {
	GFile *parent;
	gchar *name, *path;
	GList *list = NULL;

	NautilusFileInfo *info;
	NautilusMakepkg *self = NAUTILUS_MAKEPKG (provider);

	if (!gconf_client_get_bool (self->priv->conf, NAUTILUS_MAKEPKG_CONF_ENABLED, NULL)) {
		return list;
	}

	if (g_list_length (files) != 1) {
		return list;
	}

	info = NAUTILUS_FILE_INFO (files->data);
	name = nautilus_file_info_get_name (info);

	parent = nautilus_file_info_get_parent_location (info);
	path = g_file_get_path (parent);

	if (g_strcmp0 (name, "PKGBUILD") == 0 && path != NULL) {
		NautilusMenuItem *item = nautilus_makepkg_menu_item_new (self, window, path);
		list = g_list_append (list, item);
	}

	g_free (name);
	g_free (path);
	g_object_unref (parent);

	return list;
}

static GList *nautilus_makepkg_get_background_items (NautilusMenuProvider *provider, GtkWidget *window, NautilusFileInfo *current_folder) {
	return NULL;
}

static GList *nautilus_makepkg_get_toolbar_items (NautilusMenuProvider *provider, GtkWidget *window, NautilusFileInfo *current_folder) {
	return NULL;
}

static void nautilus_makepkg_class_init (NautilusMakepkgClass *klass) {
	G_OBJECT_CLASS (klass)->finalize = nautilus_makepkg_finalize;
	g_type_class_add_private (klass, sizeof (NautilusMakepkgPrivate));
}

static void menu_provider_iface_init (NautilusMenuProviderIface *iface) {
	iface->get_file_items = nautilus_makepkg_get_file_items;
	iface->get_background_items = nautilus_makepkg_get_background_items;
	iface->get_toolbar_items = nautilus_makepkg_get_toolbar_items;
}

static void nautilus_makepkg_class_finalize (NautilusMakepkgClass *klass) {
	return;
}

G_DEFINE_DYNAMIC_TYPE_EXTENDED (NautilusMakepkg, nautilus_makepkg, G_TYPE_OBJECT, 0,
G_IMPLEMENT_INTERFACE_DYNAMIC (NAUTILUS_TYPE_MENU_PROVIDER, &menu_provider_iface_init));

void nautilus_makepkg_register_type_for_module (GTypeModule *module) {
	nautilus_makepkg_register_type (module);
}
