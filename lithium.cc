#include "lithium.h"
#include <gtkmm/box.h>
#include <gtkmm/menubar.h>
#include <gtkmm/menu.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <iostream>

// window
namespace lith {
	namespace ui {

		// creates a new button with label "Hello World".
		window::window() {}

		void window::load_style(std::basic_string<char> path) {
			css = Gtk::CssProvider::get_default(); // get gtk default CssProvider
			css->load_from_path(path);
			auto screen = Gdk::Screen::get_default();
			auto ctx = Gtk::StyleContext::create();
			ctx->add_provider_for_screen(screen, css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
		}

		window::~window(){}
	}
}


// editor Window
namespace lith {
	namespace ui {
		namespace editor {

			// creates a new button with label "Hello World".
			window::window(std::basic_string<char> str) {

				set_default_size(600, 800); // set window size
				set_title("Lithium");

				// vertical packing box
				auto *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));

				// menu bar
				auto*menubar = Gtk::manage(new Gtk::MenuBar());
				vbox->pack_start(*menubar, Gtk::PACK_SHRINK, 0);

				// fill menu with items
				auto *menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
				menubar->append(*menuitem_file);
				auto *filemenu = Gtk::manage(new Gtk::Menu());
				menuitem_file->set_submenu(*filemenu);
				auto *menuitem_quit = Gtk::manage(new Gtk::MenuItem("_Quit", true));
				menuitem_quit->signal_activate().connect(sigc::mem_fun(*this, &window::on_quit_click));
				filemenu->append(*menuitem_quit);

				// keeps track of editor
				auto *editor_scroll = Gtk::manage(new Gtk::ScrolledWindow());
				editor = Gtk::manage(new textView());
				editor->set_name("editor_pane");
				editor_scroll->add(*editor);
				vbox->pack_end(*editor_scroll, Gtk::PACK_EXPAND_WIDGET, 0);

				add(*vbox);
				show_all();
			}

			window::~window(){}

			void window::on_button_click() {
				std::cout << "Hello World" << std::endl;
			}

			void window::on_quit_click() {
				hide();
			}
		}
	}
}


// editor view
namespace lith {
	namespace ui {
		namespace editor {

			textView::textView() {
				set_wrap_mode(Gtk::WrapMode::WRAP_WORD_CHAR);
			}

			textView::~textView() {}

		}
	}
}
