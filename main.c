#include <gtk/gtk.h>
#include <stdlib.h>
#include "game.h"

// create global widget variable for easy use in callback of event handler
// type GtkWindow
GtkWidget *window;
// type GtkLabel
GtkWidget *lblLevelName;
// type GtkButton
GtkWidget *btnPreviousLevel;
// type GtkButton
GtkWidget *btnReset;
// type GtkButton
GtkWidget *btnNextLevel;
// type GtkToggleButton
GtkWidget *btnNum1;
// type GtkToggleButton
GtkWidget *btnNum2;
// type GtkToggleButton
GtkWidget *btnNum3;
// type GtkToggleButton
GtkWidget *btnNum4;
// type GtkToggleButton
GtkWidget *btnPlus;
// type GtkToggleButton
GtkWidget *btnMinus;
// type GtkToggleButton
GtkWidget *btnMultiple;
// type GtkToggleButton
GtkWidget *btnDivide;
// type GtkToggleButton
GtkWidget *btn24;
// type GtkLabel
GtkWidget *lblStatus;

GtkWidget ** btnNums;
GtkWidget ** btnOperations;

/**
 * use GtkBuilder to parse, initialize widgets and connect signals
 */
void initializationWidgets() {
    // create a GtkBuilder to parse xml format ui file
    GtkBuilder *builder = gtk_builder_new();
    // see https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-add-from-file
    gtk_builder_add_from_file(builder, "main.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    lblLevelName = GTK_WIDGET(gtk_builder_get_object(builder, "lblLevelName"));
    btnPreviousLevel = GTK_WIDGET(gtk_builder_get_object(builder, "btnPreviousLevel"));
    btnReset = GTK_WIDGET(gtk_builder_get_object(builder, "btnReset"));
    btnNextLevel = GTK_WIDGET(gtk_builder_get_object(builder, "btnNextLevel"));
    btnNum1 = GTK_WIDGET(gtk_builder_get_object(builder, "btnNum1"));
    btnNum2 = GTK_WIDGET(gtk_builder_get_object(builder, "btnNum2"));
    btnNum3 = GTK_WIDGET(gtk_builder_get_object(builder, "btnNum3"));
    btnNum4 = GTK_WIDGET(gtk_builder_get_object(builder, "btnNum4"));
    btnPlus = GTK_WIDGET(gtk_builder_get_object(builder, "btnPlus"));
    btnMinus = GTK_WIDGET(gtk_builder_get_object(builder, "btnMinus"));
    btnMultiple = GTK_WIDGET(gtk_builder_get_object(builder, "btnMultiple"));
    btnDivide = GTK_WIDGET(gtk_builder_get_object(builder, "btnDivide"));
    btn24 = GTK_WIDGET(gtk_builder_get_object(builder, "btn24"));
    lblStatus = GTK_WIDGET(gtk_builder_get_object(builder, "lblStatus"));
    // connect all signals
    // see https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-connect-signals
    gtk_builder_connect_signals(builder, NULL);
    // decrease the reference count like free/delete operation
    // see https://developer.gnome.org/gobject/stable/gobject-memory.html
    g_object_unref(builder);
    // see https://developer.gnome.org/gtk3/stable/GtkWidget.html#gtk-widget-show
    gtk_widget_show(window);
}


G_MODULE_EXPORT void on_window_destroy() {
    // g_print("running on_window_destroy\n");
    gtk_main_quit();
}
G_MODULE_EXPORT void on_btnPreviousLevel_clicked() {
    // g_print("running on_btnPreviousLevel_clicked\n");
    initializationGamePreviousLevel();
}
G_MODULE_EXPORT void on_btnReset_clicked() {
    // g_print("running on_btnReset_clicked\n");
    initializationGameResetLevel();
}
G_MODULE_EXPORT void on_btnNextLevel_clicked() {
    // g_print("running on_btnNextLevel_clicked\n");
    initializationGameNextLevel();
}
G_MODULE_EXPORT void on_btnNum1_toggled() {
    // g_print("running on_btnNum1_toggled\n");
    btnNumCommon(0);
}
G_MODULE_EXPORT void on_btnNum2_toggled() {
    // g_print("running on_btnNum2_toggled\n");
    btnNumCommon(1);
}
G_MODULE_EXPORT void on_btnNum3_toggled() {
    // g_print("running on_btnNum3_toggled\n");
    btnNumCommon(2);
}
G_MODULE_EXPORT void on_btnNum4_toggled() {
    // g_print("running on_btnNum4_toggled\n");
    btnNumCommon(3);
}
G_MODULE_EXPORT void on_btnPlus_toggled() {
    // g_print("running on_btnPlus_toggled\n");
    btnOperationCommon(0);
}
G_MODULE_EXPORT void on_btnMinus_toggled() {
    // g_print("running on_btnMinus_toggled\n");
    btnOperationCommon(1);
}
G_MODULE_EXPORT void on_btnMultiple_toggled() {
    // g_print("running on_btnMultiple_toggled\n");
    btnOperationCommon(2);
}
G_MODULE_EXPORT void on_btnDivide_toggled() {
    // g_print("running on_btnDivide_toggled\n");
    btnOperationCommon(3);
}

int main(int argc, char *argv[]) {
    // see https://developer.gnome.org/gtk3/stable/gtk3-General.html#gtk-init
    gtk_init(&argc, &argv);
    initializationWidgets();
    // start game
    initializationGame();
    initializationGameLevel(0);
    // see https://developer.gnome.org/gtk3/stable/gtk3-General.html#gtk-main
    gtk_main();
    return 0;
}