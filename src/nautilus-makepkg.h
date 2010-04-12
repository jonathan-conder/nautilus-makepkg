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

#ifndef __NAUTILUS_MAKEPKG_H__
#define __NAUTILUS_MAKEPKG_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define NAUTILUS_TYPE_MAKEPKG (nautilus_makepkg_get_type ())
#define NAUTILUS_MAKEPKG(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), NAUTILUS_TYPE_MAKEPKG, NautilusMakepkg))
#define NAUTILUS_MAKEPKG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), NAUTILUS_TYPE_MAKEPKG, NautilusMakepkgClass))
#define NAUTILUS_IS_MAKEPKG(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NAUTILUS_TYPE_MAKEPKG))
#define NAUTILUS_IS_MAKEPKG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), NAUTILUS_TYPE_MAKEPKG))
#define NAUTILUS_MAKEPKG_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), NAUTILUS_TYPE_MAKEPKG, NautilusMakepkgClass))

typedef struct _NautilusMakepkg NautilusMakepkg;
typedef struct _NautilusMakepkgClass NautilusMakepkgClass;
typedef struct _NautilusMakepkgPrivate NautilusMakepkgPrivate;

struct _NautilusMakepkg {
	GObject parent_instance;
	NautilusMakepkgPrivate *priv;
};

struct _NautilusMakepkgClass {
	GObjectClass parent_class;
};

GType nautilus_makepkg_get_type (void);
void nautilus_makepkg_register_type_for_module (GTypeModule *module);

G_END_DECLS

#endif
