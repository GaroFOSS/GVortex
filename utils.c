#include "utils.h"
#ifdef _WIN32
#include <windows.h> // Required for GetModuleFileName
#endif

// Loads the CSS file and applies it to the application.
void load_css(void) {
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;
    gchar exe_path[MAX_PATH];
    gchar *exe_dir;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GetModuleFileName(NULL, exe_path, MAX_PATH);
    exe_dir = g_path_get_dirname(exe_path);

    gchar *css_path = g_build_filename(exe_dir, "style.css", NULL);

    GError *error = NULL;
    if (!gtk_css_provider_load_from_path(provider, css_path, &error)) {
        g_warning("Error loading CSS file: %s", error->message);
        g_clear_error(&error);
    }
    
    g_free(exe_dir);
    g_free(css_path);
    g_object_unref(provider);
}