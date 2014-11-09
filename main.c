
#include "gui.h"

int main() {
	pthread_t ui_worker = lithium_ui();

	// other code can run on this thread.

	pthread_join(ui_worker, NULL);
}
