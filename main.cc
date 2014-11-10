#include "lithium.h"

int main(int argc, char *argv[]) {

	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

	lith::ui::window window(std::basic_string<char>("hello"));

 	//Shows the window and returns when it is closed.
 	return app->run(window);
}
