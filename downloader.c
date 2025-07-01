#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "downloader.h"
#include "utils.h"

#ifdef _WIN32
    #define POPEN _popen
    #define PCLOSE _pclose
#else
    #define POPEN popen
    #define PCLOSE pclose
#endif

static gboolean update_status_from_thread(gpointer user_data);
static gboolean update_progress_from_thread(gpointer user_data);
static gboolean on_download_finished(gpointer user_data);
static void update_status_log(const gchar *message, GtkTextView *text_view);


// Main function for the download thread.
gpointer download_thread_func(gpointer data) {
    ThreadData *thread_data = (ThreadData *)data;
    AppWidgets *widgets = thread_data->widgets;
    int success_count = 0;
    int total_urls = 0;
    while(thread_data->urls[total_urls] != NULL) total_urls++;

    for (int i = 0; thread_data->urls[i] != NULL; ++i) {
        gchar *url = thread_data->urls[i];
        if (g_str_equal(url, "")) continue;

        ProgressUpdate *reset_progress = g_new(ProgressUpdate, 1);
        reset_progress->widgets = widgets;
        reset_progress->fraction = 0.0;
        reset_progress->text = g_strdup("Starting...");
        g_idle_add(update_progress_from_thread, reset_progress);
        
        gchar *progress_msg = g_strdup_printf("\n--- [%d/%d] Downloading: %s... ---", i + 1, total_urls, url);
        StatusUpdate *start_update = g_new(StatusUpdate, 1);
        start_update->text_view = GTK_TEXT_VIEW(widgets->status_view);
        start_update->message = progress_msg;
        g_idle_add(update_status_from_thread, start_update);

        gchar *command;
        if (g_strcmp0(thread_data->download_type, "video") == 0) {
            command = g_strdup_printf("yt-dlp.exe --newline -f \"bestvideo+bestaudio/best\" --merge-output-format mp4 -o \"%s\\%%(title)s.%%(ext)s\" \"%s\"", thread_data->output_path, url);
        } else {
            command = g_strdup_printf("yt-dlp.exe --newline -x --audio-format mp3 --audio-quality 0 -o \"%s\\%%(title)s.%%(ext)s\" \"%s\"", thread_data->output_path, url);
        }

        FILE *fp = POPEN(command, "r");
        g_free(command);
        if (fp == NULL) {
            StatusUpdate *err_update = g_new(StatusUpdate, 1);
            err_update->text_view = GTK_TEXT_VIEW(widgets->status_view);
            err_update->message = g_strdup("ERROR: Failed to run yt-dlp.exe. Is it in your PATH?");
            g_idle_add(update_status_from_thread, err_update);
            continue;
        }

        char line[1024];
        while (fgets(line, sizeof(line), fp) != NULL) {
            line[strcspn(line, "\r\n")] = 0;
            if (g_str_has_prefix(line, "[download]")) {
                double percent;
                if (sscanf(line, "[download] %lf%%", &percent) == 1) {
                    char *eta_ptr = strstr(line, "ETA");
                    char *eta_str = eta_ptr ? eta_ptr : "ETA --:--";
                    ProgressUpdate *progress = g_new(ProgressUpdate, 1);
                    progress->widgets = widgets;
                    progress->fraction = percent / 100.0;
                    progress->text = g_strdup_printf("%.1f%%  |  %s", percent, eta_str);
                    g_idle_add(update_progress_from_thread, progress);
                } else {
                    StatusUpdate *line_update = g_new(StatusUpdate, 1);
                    line_update->text_view = GTK_TEXT_VIEW(widgets->status_view);
                    line_update->message = g_strdup(line);
                    g_idle_add(update_status_from_thread, line_update);
                }
            } else {
                StatusUpdate *line_update = g_new(StatusUpdate, 1);
                line_update->text_view = GTK_TEXT_VIEW(widgets->status_view);
                line_update->message = g_strdup(line);
                g_idle_add(update_status_from_thread, line_update);
            }
        }

        int status = PCLOSE(fp);
        gchar *end_msg;
        if (status == 0) {
            success_count++;
            end_msg = g_strdup("--- Download finished successfully. ---");
            ProgressUpdate *final_progress = g_new(ProgressUpdate, 1);
            final_progress->widgets = widgets;
            final_progress->fraction = 1.0;
            final_progress->text = g_strdup("Completed!");
            g_idle_add(update_progress_from_thread, final_progress);
        } else {
            end_msg = g_strdup_printf("--- Download failed (exit code: %d). ---", status);
        }

        StatusUpdate *end_update = g_new(StatusUpdate, 1);
        end_update->text_view = GTK_TEXT_VIEW(widgets->status_view);
        end_update->message = end_msg;
        g_idle_add(update_status_from_thread, end_update);
    }

    gchar *summary_msg = g_strdup_printf("\n--- Bulk process finished. Downloaded %d of %d items successfully. ---", success_count, total_urls);
    StatusUpdate *summary_update = g_new(StatusUpdate, 1);
    summary_update->text_view = GTK_TEXT_VIEW(widgets->status_view);
    summary_update->message = summary_msg;
    g_idle_add(update_status_from_thread, summary_update);

    g_idle_add(on_download_finished, widgets);

    g_strfreev(thread_data->urls);
    g_free(thread_data->output_path);
    g_free(thread_data->download_type);
    g_free(thread_data);

    return NULL;
}

static gboolean update_status_from_thread(gpointer user_data) {
    StatusUpdate *update_data = (StatusUpdate *)user_data;
    update_status_log(update_data->message, update_data->text_view);
    g_free(update_data->message);
    g_free(update_data);
    return G_SOURCE_REMOVE;
}

static gboolean update_progress_from_thread(gpointer user_data) {
    ProgressUpdate *progress = (ProgressUpdate *)user_data;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress->widgets->progress_bar), progress->fraction);
    gtk_label_set_text(GTK_LABEL(progress->widgets->progress_label), progress->text);
    g_free(progress->text);
    g_free(progress);
    return G_SOURCE_REMOVE;
}

static void update_status_log(const gchar *message, GtkTextView *text_view) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    GtkTextIter end_iter;
    gtk_text_buffer_get_end_iter(buffer, &end_iter);
    gtk_text_buffer_insert(buffer, &end_iter, message, -1);
    gtk_text_buffer_insert(buffer, &end_iter, "\n", -1);
    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(
        GTK_SCROLLED_WINDOW(gtk_widget_get_parent(GTK_WIDGET(text_view))));
    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));
}

static gboolean on_download_finished(gpointer user_data) {
    AppWidgets *widgets = (AppWidgets *)user_data;
    gtk_widget_set_sensitive(widgets->download_button, TRUE);
    gtk_spinner_stop(GTK_SPINNER(widgets->spinner));
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->progress_bar), 0);
    gtk_label_set_text(GTK_LABEL(widgets->progress_label), "");
    return G_SOURCE_REMOVE;
}