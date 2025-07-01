#include "callbacks.h"
#include "downloader.h"

void on_start_download(GtkButton *button, gpointer user_data) {
    AppWidgets *widgets = (AppWidgets *)user_data;

    GtkTextBuffer *url_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->url_view));
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(url_buffer, &start);
    gtk_text_buffer_get_end_iter(url_buffer, &end);
    gchar *urls_text = gtk_text_buffer_get_text(url_buffer, &start, &end, FALSE);

    if (g_strcmp0(urls_text, "") == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(widgets->window),
            GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Please enter at least one URL.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        g_free(urls_text);
        return;
    }

    gtk_widget_set_sensitive(widgets->download_button, FALSE);
    gtk_spinner_start(GTK_SPINNER(widgets->spinner));
    GtkTextBuffer *status_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->status_view));
    gtk_text_buffer_set_text(status_buffer, "", -1);

    ThreadData *thread_data = g_new(ThreadData, 1);
    thread_data->urls = g_strsplit(urls_text, "\n", -1);
    thread_data->output_path = g_strdup(gtk_entry_get_text(GTK_ENTRY(widgets->output_path_entry)));
    thread_data->download_type = g_strdup(
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->download_type_video)) ? "video" : "audio");
    thread_data->widgets = widgets;

    // Create and start the thread.
    GThread *thread = g_thread_new("download_worker", download_thread_func, thread_data);
    g_thread_unref(thread);

    g_free(urls_text);
}

void on_browse_clicked(GtkButton *button, gpointer user_data) {
    AppWidgets *widgets = (AppWidgets *)user_data;
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;

    dialog = gtk_file_chooser_dialog_new("Select Output Folder",
                                         GTK_WINDOW(widgets->window),
                                         action,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Select", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *foldername = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        gtk_entry_set_text(GTK_ENTRY(widgets->output_path_entry), foldername);
        g_free(foldername);
    }
    gtk_widget_destroy(dialog);
}