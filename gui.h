
// graphical interface function header

#ifndef LITHIUM_GUI_H_
#define LITHIUM_GUI_H_

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

void lithium_gui_init();

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
	GtkWidget *dialog;

	dialog = gtk_file_chooser_dialog_new ("Open File", GTK_WINDOW(gtk_widget_get_toplevel (widget)), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		printf("opened file: %s\n", filename); // TODO: actually open file in viewer
		g_free (filename);
	}

	gtk_widget_destroy (dialog);
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

typedef struct {
	GtkWidget *view;
	GtkTextBuffer *buf;
} gui_textbox;

gui_textbox *gui_textbox_init() {
	gui_textbox *tb = malloc(sizeof(gui_textbox));

	tb->view = gtk_text_view_new ();
	tb->buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tb->view));

	gtk_text_buffer_set_text (tb->buf, "Hello, this is some text", -1);

	// change text font
	PangoFontDescription *font = pango_font_description_from_string ("Ubuntu 10");
	gtk_widget_modify_font (tb->view, font);
	pango_font_description_free (font);

	GdkColor color;
	gdk_color_parse ("#212121", &color);
	gtk_widget_modify_base(GTK_WIDGET(tb->view), GTK_STATE_NORMAL, &color);

	return tb;
}

GtkWidget *gui_menubar_init() {
	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);

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

void gui_init() {
	GtkWidget *w = gui_window_init();

	// box for vertical packing
	GtkWidget *box = gtk_vbox_new(FALSE, 0);

	gui_textbox *tb = gui_textbox_init();
	GtkWidget *mb = gui_menubar_init();

	// append to box
	gtk_box_pack_start(GTK_BOX(box), mb, FALSE, FALSE, 0); // takes up little space
	gtk_box_pack_end(GTK_BOX(box), tb->view, TRUE, TRUE, 0); // takes up max space

	// append box to window
	gtk_container_add (GTK_CONTAINER(w), box);

	// set window color (useless if there is no window visible)
	//GdkColor color;
	//gdk_color_parse ("#1f1f1f", &color);
	//gtk_widget_modify_bg (GTK_WIDGET(w), GTK_STATE_NORMAL, &color);

	// display button & window
	gtk_widget_show_all(w);

	gtk_main(); // wait for event
}

void lithium_gui_init() {

	// initialize gtk
	gtk_init(0, NULL);
	gui_init();

	// TODO: init libnotify & other gui libs
}

#endif // LITHIUM_GUI_H_
