#ifndef APP_WIDGETS_H
#define APP_WIDGETS_H

#include <gtk/gtk.h>

// A struct to hold pointers to all the widgets we need to access globally.
typedef struct {
    GtkWidget *window;
    GtkWidget *url_view;
    GtkWidget *output_path_entry;
    GtkWidget *download_type_video;
    GtkWidget *status_view;
    GtkWidget *download_button;
    GtkWidget *spinner;
    GtkWidget *progress_bar;
    GtkWidget *progress_label;
} AppWidgets;

#endif // APP_WIDGETS_H