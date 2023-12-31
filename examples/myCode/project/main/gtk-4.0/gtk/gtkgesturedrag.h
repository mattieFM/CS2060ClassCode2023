/* GTK - The GIMP Toolkit
 * Copyright (C) 2014, Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author(s): Carlos Garnacho <carlosg@gnome.org>
 */
#pragma once

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gtk/gtkwidget.h>
#include <gtk/gtkgesturesingle.h>

G_BEGIN_DECLS

#define GTK_TYPE_GESTURE_DRAG         (gtk_gesture_drag_get_type ())
#define GTK_GESTURE_DRAG(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GTK_TYPE_GESTURE_DRAG, GtkGestureDrag))
#define GTK_GESTURE_DRAG_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k), GTK_TYPE_GESTURE_DRAG, GtkGestureDragClass))
#define GTK_IS_GESTURE_DRAG(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GTK_TYPE_GESTURE_DRAG))
#define GTK_IS_GESTURE_DRAG_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), GTK_TYPE_GESTURE_DRAG))
#define GTK_GESTURE_DRAG_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GTK_TYPE_GESTURE_DRAG, GtkGestureDragClass))

typedef struct _GtkGestureDrag GtkGestureDrag;
typedef struct _GtkGestureDragClass GtkGestureDragClass;

GDK_AVAILABLE_IN_ALL
GType        gtk_gesture_drag_get_type          (void) G_GNUC_CONST;

GDK_AVAILABLE_IN_ALL
GtkGesture * gtk_gesture_drag_new               (void);

GDK_AVAILABLE_IN_ALL
gboolean     gtk_gesture_drag_get_start_point   (GtkGestureDrag *gesture,
                                                 double         *x,
                                                 double         *y);
GDK_AVAILABLE_IN_ALL
gboolean     gtk_gesture_drag_get_offset        (GtkGestureDrag *gesture,
                                                 double         *x,
                                                 double         *y);

G_DEFINE_AUTOPTR_CLEANUP_FUNC(GtkGestureDrag, g_object_unref)

G_END_DECLS

