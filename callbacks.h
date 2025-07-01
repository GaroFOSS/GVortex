#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>
#include "app_widgets.h"

// Callback for the "Download" button.
void on_start_download(GtkButton *button, gpointer user_data);

// Callback for the "Browse" button.
void on_browse_clicked(GtkButton *button, gpointer user_data);

#endif // CALLBACKS_H