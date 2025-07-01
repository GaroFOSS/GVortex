#ifndef UTILS_H
#define UTILS_H

#include <gtk/gtk.h>
#include "app_widgets.h"

void load_css(void);

typedef struct {
    AppWidgets *widgets;
    gdouble fraction;
    gchar *text;
} ProgressUpdate;

typedef struct {
    GtkTextView *text_view;
    gchar *message;
} StatusUpdate;

#endif // UTILS_H