#include "lithium.h"

int main(int argc, char *argv[]) {

	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

	lith::ui::editor::window window(std::basic_string<char>("hello"));

	window.load_style("style/lithium.css");

 	//Shows the window and returns when it is closed.
 	return app->run(window);
}
