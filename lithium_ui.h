#ifndef LITHIUM_LITHIUM_UI_H_
#define LITHIUM_LITHIUM_UI_H_

#include <gtk/gtk.h>

// lithium_ui type
// store all "global" info in this struct
typedef struct {
	GtkApplication *app; // main window
} lithium_ui;

// initiate library
lithium_ui *lithium_ui_init();

// run library event loop
int lithium_ui_run(lithium_ui *);

void lithium_ui_free(lithium_ui *);

#endif // LITHIUM_LITHIUM_UI_H_
