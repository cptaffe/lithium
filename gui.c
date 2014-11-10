#include <pthread.h>
#include "gui.h"
#include "lithium_ui.h"

// thread ui worker, controls ui.
static void *ui_worker(void *s) {
	// runs gtk loop
	lithium_ui *lith = lithium_ui_init("style/lithium.css");
	lithium_ui_run(lith);
	lithium_ui_free(lith);
	return NULL;
}

// starts a thread to work the ui
// TODO: process qued work in a producer-consumer relationship.
pthread_t lithium() {
	pthread_t ui_thread;
	pthread_create(&ui_thread, NULL, &ui_worker, NULL); // wait for event
	return ui_thread;
}
