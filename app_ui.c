#include "app_ui.h"
#include "app_widgets.h"
#include "callbacks.h"
#include "utils.h"

void build_ui(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GVortex Downloader");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 650);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    load_css();

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    AppWidgets *widgets = g_new(AppWidgets, 1);
    widgets->window = window;

    GtkWidget *urls_label = gtk_label_new("Video URLs (one per line):");
    gtk_widget_set_halign(urls_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), urls_label, 0, 0, 2, 1);

    GtkWidget *url_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(url_scrolled_window, TRUE);
    gtk_widget_set_vexpand(url_scrolled_window, TRUE);
    widgets->url_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(url_scrolled_window), widgets->url_view);
    gtk_grid_attach(GTK_GRID(grid), url_scrolled_window, 0, 1, 2, 1);

    GtkWidget *output_label = gtk_label_new("Save to:");
    gtk_widget_set_halign(output_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), output_label, 0, 2, 2, 1);

    widgets->output_path_entry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(widgets->output_path_entry), FALSE);
    gtk_entry_set_text(GTK_ENTRY(widgets->output_path_entry), g_get_current_dir());
    gtk_widget_set_hexpand(widgets->output_path_entry, TRUE);
    gtk_grid_attach(GTK_GRID(grid), widgets->output_path_entry, 0, 3, 1, 1);
    
    GtkWidget *browse_button = gtk_button_new_with_label("Browse...");
    g_signal_connect(browse_button, "clicked", G_CALLBACK(on_browse_clicked), widgets);
    gtk_grid_attach(GTK_GRID(grid), browse_button, 1, 3, 1, 1);
    
    GtkWidget *type_label = gtk_label_new("Download Type:");
    gtk_widget_set_halign(type_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), type_label, 0, 4, 2, 1);

    widgets->download_type_video = gtk_radio_button_new_with_label(NULL, "Video (.mp4)");
    gtk_grid_attach(GTK_GRID(grid), widgets->download_type_video, 0, 5, 2, 1);
    GtkWidget* download_type_audio = gtk_radio_button_new_with_label_from_widget(
        GTK_RADIO_BUTTON(widgets->download_type_video), "Audio Only (.mp3)");
    gtk_grid_attach(GTK_GRID(grid), download_type_audio, 0, 6, 2, 1);

    GtkWidget *download_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    widgets->download_button = gtk_button_new_with_label("Download All");
    gtk_widget_set_name(widgets->download_button, "download-button");
    gtk_widget_set_hexpand(widgets->download_button, TRUE);
    g_signal_connect(widgets->download_button, "clicked", G_CALLBACK(on_start_download), widgets);
    widgets->spinner = gtk_spinner_new();
    gtk_box_pack_start(GTK_BOX(download_box), widgets->download_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(download_box), widgets->spinner, FALSE, FALSE, 0);
    gtk_grid_attach(GTK_GRID(grid), download_box, 0, 7, 2, 1);

    widgets->progress_bar = gtk_progress_bar_new();
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(widgets->progress_bar), FALSE);
    gtk_grid_attach(GTK_GRID(grid), widgets->progress_bar, 0, 8, 1, 1);

    widgets->progress_label = gtk_label_new("");
    gtk_widget_set_halign(widgets->progress_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), widgets->progress_label, 1, 8, 1, 1);

    GtkWidget *status_label = gtk_label_new("Status:");
    gtk_widget_set_halign(status_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), status_label, 0, 9, 2, 1);
    
    GtkWidget *status_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(status_scrolled_window, TRUE);
    gtk_widget_set_vexpand(status_scrolled_window, TRUE);
    widgets->status_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(widgets->status_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(widgets->status_view), FALSE);
    gtk_container_add(GTK_CONTAINER(status_scrolled_window), widgets->status_view);
    gtk_grid_attach(GTK_GRID(grid), status_scrolled_window, 0, 10, 2, 1);

    gtk_widget_show_all(window);
}