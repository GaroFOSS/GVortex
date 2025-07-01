#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <glib.h>
#include "app_widgets.h"

// A struct to pass all necessary data to the download thread.
typedef struct {
    gchar **urls;
    gchar *output_path;
    gchar *download_type;
    AppWidgets *widgets;
} ThreadData;

gpointer download_thread_func(gpointer data);

#endif // DOWNLOADER_H