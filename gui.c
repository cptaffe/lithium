#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include "gui.h"

// globals
static char *editor_color = "#212121";

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

void open_file(GtkWidget *widget) {
	/*
	GtkWidget *dialog;

	dialog = gtk_file_chooser_dialog_new ("Open File", GTK_WINDOW(gtk_widget_get_toplevel (widget)), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		printf("opened file: %s\n", filename); // TODO: actually open file in viewer
		g_free (filename);
	}

	gtk_widget_destroy (dialog);
	*/
}

GtkWidget *gui_window_init() {
	GtkWidget *w = gtk_window_new (GTK_WINDOW_TOPLEVEL);

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

GtkWidget *gui_textbox_tb_init() {
	GtkWidget *view = gtk_text_view_new();
	GtkTextBuffer *buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	gtk_text_buffer_set_text (buf, "Hello, this is some text", -1);

	// change text font
	PangoFontDescription *font = pango_font_description_from_string ("Ubuntu 10");
	gtk_widget_override_font(view, font);
	pango_font_description_free (font);

	GdkRGBA color;
	gdk_rgba_parse (&color, editor_color);
	gtk_widget_override_background_color(GTK_WIDGET(view), GTK_STATE_FLAG_NORMAL, &color);

	gdk_rgba_parse (&color, "white");
	gtk_widget_override_color(GTK_WIDGET(view), GTK_STATE_FLAG_NORMAL, &color);

	return view;
}

GtkWidget *gui_textbox_init() {

	GtkWidget *notebook = gtk_notebook_new();

	gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook), TRUE); // scroll through tabs
	gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);

	GtkWidget *tab_label = gtk_label_new("Unknown"); // tabl label

	GdkRGBA color;
	gdk_rgba_parse (&color, editor_color);
	gtk_widget_override_background_color(GTK_WIDGET(notebook), GTK_STATE_FLAG_NORMAL, &color);

	gdk_rgba_parse (&color, "white");
	gtk_widget_override_color(GTK_WIDGET(notebook), GTK_STATE_FLAG_NORMAL, &color);
	gtk_widget_override_color(GTK_WIDGET(tab_label), GTK_STATE_FLAG_NORMAL, &color);

	GtkWidget *view = gui_textbox_tb_init();

	// scrollbar
	GtkWidget* scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_kinetic_scrolling (GTK_SCROLLED_WINDOW(scrolledwindow), TRUE);

	gtk_container_add(GTK_CONTAINER(scrolledwindow), view);

	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scrolledwindow, tab_label);

	return notebook;
}

GtkWidget *gui_menubar_init() {
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

GtkWidget *gui_basebar_init() {
	return NULL;
}

void gui_init() {
	GtkWidget *w = gui_window_init();

	// box for vertical packing
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	GtkWidget *tb = gui_textbox_init();
	GtkWidget *mb = gui_menubar_init();

	// append to box
	gtk_box_pack_start(GTK_BOX(box), mb, FALSE, FALSE, 0); // takes up little space
	gtk_box_pack_end(GTK_BOX(box), tb, TRUE, TRUE, 0); // takes up max space

	// append box to window
	gtk_container_add (GTK_CONTAINER(w), box);

	// set window color (useless if there is no window visible)
	GdkRGBA color;
	gdk_rgba_parse (&color, "#1f1f1f");
	gtk_widget_override_background_color(GTK_WIDGET(w), GTK_STATE_FLAG_NORMAL, &color);

	// display button & window
	gtk_widget_show_all(w);
}

void lithium_gui_init() {

	// initialize gtk
	gtk_init(0, NULL);
	gui_init();

	// TODO: init libnotify & other gui libs
}

void lithium_gui_finalize() {
	gtk_main(); // wait for event
}
