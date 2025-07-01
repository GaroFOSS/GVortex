#include "app_ui.h"

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    
    app = gtk_application_new("com.fossgaro.gvortex", G_APPLICATION_DEFAULT_FLAGS);

    // Connect the "activate" signal to the UI-building function.
    g_signal_connect(app, "activate", G_CALLBACK(build_ui), NULL);

    // Run application
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}