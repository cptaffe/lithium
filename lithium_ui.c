// lithium UI abstraction layer

#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>
#include <stdlib.h>
#include <stdio.h>
#include "lithium_ui.h"

// button callback
static void hello( GtkWidget *widget, gpointer   data ) {
	g_print ("Hello World\n");
}

// delete event callback
static gboolean delete_event(GtkWidget *widget, GdkEvent  *event, gpointer   data )
{
	// printf ("delete event occurred\n");
	return gtk_false(); // don't destroy
}

static void destroy(GtkWidget *widget, gpointer data ) {
	gtk_main_quit ();
}

GtkWidget *ui_window_init(GtkApplication *app) {
	GtkWidget *w = gtk_application_window_new (app);

	// window title
	gtk_window_set_title(GTK_WINDOW(w), "lithium!");

	// position and size
	gtk_window_set_position(GTK_WINDOW(w), GTK_WIN_POS_MOUSE);
	gtk_window_set_default_size(GTK_WINDOW(w), 600, 800);

	// register event bindings
	g_signal_connect (w, "delete-event", G_CALLBACK(delete_event), NULL);
	g_signal_connect (w, "destroy", G_CALLBACK(destroy), NULL);

	// set border size of window
	gtk_container_set_border_width (GTK_CONTAINER(w), 0);

	return w;
}

GtkWidget *ui_textbox_tb_init() {
	GtkSourceBuffer *buf = gtk_source_buffer_new(NULL);

	// Filling buffer is not undo-able
	gtk_source_buffer_begin_not_undoable_action(buf);
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buf), "Hello, this is some text", -1);
	gtk_source_buffer_end_not_undoable_action(buf);

	GtkWidget *view = gtk_source_view_new_with_buffer(buf);

	gtk_widget_set_name(view, "lithium_src");

	return view;
}

void open_file(GtkWidget *widget) {

	char *filename = NULL;

	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open File", GTK_WINDOW(gtk_widget_get_toplevel (widget)), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", "_Cancel", "_Open", GTK_RESPONSE_ACCEPT, NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
	} else {
		return;
	}
	gtk_widget_destroy (dialog);


	GtkWidget *window = gtk_widget_get_toplevel(widget);
	// TODO: get buf from window
	GtkSourceBuffer *buf = GTK_SOURCE_BUFFER(window); // silence compile error

	// Open file in viewer
	gboolean result_uncertain;
	gchar *content_type = g_content_type_guess (filename, NULL, 0, &result_uncertain);
	if (result_uncertain) {
		g_free (content_type);
		content_type = NULL;
	}

	char *dirlist[] = {"lang"};

	GtkSourceLanguageManager *mngr = gtk_source_language_manager_get_default();
	gtk_source_language_manager_set_search_path(mngr, (char **) &dirlist); // look for language files here

	GtkSourceLanguage *lang = gtk_source_language_manager_guess_language (mngr, filename, content_type);
	gtk_source_buffer_set_language (buf, lang);

	g_free (content_type);
	g_free (filename);
}

GtkWidget *ui_textbox_init() {

	GtkWidget *view = ui_textbox_tb_init();

	// scrollbar
	GtkWidget* scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_kinetic_scrolling (GTK_SCROLLED_WINDOW(scrolledwindow), TRUE);

	gtk_container_add(GTK_CONTAINER(scrolledwindow), view);

	return scrolledwindow;
}

GtkWidget *ui_label_init(char *name) {
	GtkWidget *tab_label = gtk_label_new(name); // tabl label

	return tab_label;
}

GtkWidget *ui_notebook_tb_init() {
	GtkWidget *notebook = gtk_notebook_new();

	gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook), TRUE); // scroll through tabs
	gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
	// gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);

	for (int i = 0; i < 2; i++) {
		GtkWidget *sw = ui_textbox_init();
		char str[100];

		sprintf(&str[0], "tab #%d", i);

		gtk_notebook_append_page(GTK_NOTEBOOK(notebook), sw, ui_label_init(str));
	}

	gtk_widget_set_name(notebook, "lithium_nb"); // css selector #lithium_nb

	return notebook;
}

GtkWidget *ui_editor_init() {
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	GtkWidget *nbk = ui_notebook_tb_init();

	gtk_box_pack_start(GTK_BOX(box), nbk, TRUE, TRUE, 0);

	return box;
}

GtkWidget *ui_menubar_init() {
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	GtkWidget *menubar = gtk_menu_bar_new();
	GtkWidget *filemenu = gtk_menu_new();

	GtkWidget *file = gtk_menu_item_new_with_label("File");
	GtkWidget *nwin = gtk_menu_item_new_with_label("Open");
	GtkWidget *quit = gtk_menu_item_new_with_label("Quit");

	g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(destroy), NULL);
	g_signal_connect(G_OBJECT(nwin), "activate", G_CALLBACK(open_file), NULL);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), nwin);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

	return vbox;
}


// Loads icon files and adds them as bultin icons
void ui_load_icon(char *path, char *name) {
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(path, NULL);
	int width, height;
	gdk_pixbuf_get_file_info (path, &width, &height);
	gtk_icon_theme_add_builtin_icon (name, width, pixbuf);
	g_object_unref (G_OBJECT (pixbuf));
}

// Loads styling for ui elements (CSS)
void ui_load_css_style(char *path) {
	GtkCssProvider *provider = gtk_css_provider_new ();
	GdkDisplay *display = gdk_display_get_default ();
	GdkScreen *screen = gdk_display_get_default_screen (display);

	gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	GFile* css_file = g_file_new_for_path(path);
	gtk_css_provider_load_from_file(GTK_CSS_PROVIDER(provider), css_file, NULL);
	g_object_unref (provider);
}

GtkWidget *ui_basebar_init() {
	return NULL;
}

GtkWidget *window_init(GtkApplication *app) {

	ui_load_css_style("style/lithium.css");

	GtkWidget *w = ui_window_init(app);

	// box for vertical packing
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	GtkWidget *tb = ui_editor_init();
	GtkWidget *mb = ui_menubar_init();

	// append to box
	gtk_box_pack_start(GTK_BOX(box), mb, FALSE, FALSE, 0); // takes up little space
	gtk_box_pack_end(GTK_BOX(box), tb, TRUE, TRUE, 0); // takes up max space

	// append box to window
	gtk_container_add (GTK_CONTAINER(w), box);

	return w;
}

// run by application when it starts.
void ui_app_start(GApplication *app, gpointer user_data) {
	GtkWidget *w = window_init(GTK_APPLICATION(app));
	gtk_widget_show_all(w);
}

GtkApplication *ui_application_init(GCallback func) {
	GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (func), NULL);
	return app;
}

// Application interfacing using gtk_application

// runs the application
int lithium_ui_run(lithium_ui *ui) {
	// ignore command line arguments
	int status = g_application_run (G_APPLICATION (ui->app), 0, NULL);
	return status;
}

void lithium_ui_free(lithium_ui *ui) {
	g_object_unref(ui->app); // unreference
	ui->app = NULL;
	free(ui);
}

// utility function that inits gtk and the initial window.
lithium_ui *lithium_ui_init(char *csspath) {

	lithium_ui *lith = malloc(sizeof(lithium_ui));

	lith->app = ui_application_init(G_CALLBACK(ui_app_start));

	// boss css loading!
	ui_load_css_style(csspath);

	// TODO: init libnotify & other gui libs
	// TODO: server & client editing interface
	// TODO: editing and stuffs

	return lith;
}
