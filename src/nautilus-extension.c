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

#include <gmodule.h>

#include "nautilus-makepkg.h"

static GType types[1];

G_MODULE_EXPORT void nautilus_module_initialize (GTypeModule *module) {
	nautilus_makepkg_register_type_for_module (module);
	types[0] = NAUTILUS_TYPE_MAKEPKG;
}

G_MODULE_EXPORT void nautilus_module_shutdown (void) {
	return;
}

G_MODULE_EXPORT void nautilus_module_list_types (const GType **list, gint *length) {
	*list = types;
	*length = G_N_ELEMENTS (types);
}
