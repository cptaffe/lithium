#include "lithium.h"
#include <gtkmm/menubar.h>
#include <gtkmm/menu.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/entry.h>
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

				// set up keyboard shortcuts
				add_events( Gdk::KEY_PRESS_MASK );
				signal_key_press_event().connect ( sigc::mem_fun(*this, &window::on_key_press) );
				signal_key_release_event().connect ( sigc::mem_fun(*this, &window::on_key_release) );

				// vertical packing box
				auto *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
				add(*vbox);
				vbox->show();

				// menu bar
				auto*menubar = Gtk::manage(new Gtk::MenuBar());
				vbox->pack_start(*menubar, Gtk::PACK_SHRINK, 0);
				menubar->show();

				// File menu
				auto *menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
				menubar->append(*menuitem_file);
				menuitem_file->show();
				auto *filemenu = Gtk::manage(new Gtk::Menu());
				menuitem_file->set_submenu(*filemenu);
				filemenu->show();

				// File -> Open
				auto *menuitem_open = Gtk::manage(new Gtk::MenuItem("_Open", true));
				menuitem_open->signal_activate().connect(sigc::mem_fun(*this, &window::on_open_click));
				filemenu->append(*menuitem_open);
				menuitem_open->show();

				// File -> Quit
				auto *menuitem_quit = Gtk::manage(new Gtk::MenuItem("_Quit", true));
				menuitem_quit->signal_activate().connect(sigc::mem_fun(*this, &window::on_quit_click));
				filemenu->append(*menuitem_quit);
				menuitem_quit->show();

				// command interface
				command = Gtk::manage(new commandPalette());
				vbox->pack_start(*command, Gtk::PACK_SHRINK, 0);

				// vertical packing box
				auto *hbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
				vbox->pack_end(*hbox, Gtk::PACK_EXPAND_WIDGET, 0);
				hbox->show();

				// keeps track of editor
				auto *editor_scroll = Gtk::manage(new Gtk::ScrolledWindow());
				editor_scroll->set_kinetic_scrolling(true);
				editor = Gtk::manage(new textView());
				editor->set_name("editor_pane");
				editor_scroll->add(*editor);
				hbox->pack_start(*editor_scroll, Gtk::PACK_EXPAND_WIDGET, 0);
				editor_scroll->show();
				editor->show();


				show();
			}

			window::~window(){}

			void window::on_open_click() {
				std::cout << "Hello World" << std::endl;
			}

			// do an action on a key press
			bool window::on_key_press(GdkEventKey *event) {
				if (event->keyval == GDK_KEY_Escape) {
					hide();
				}
				return true;
			}

			// do an action on a key release
			bool window::on_key_release(GdkEventKey *event) {
				if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_P) {
					if (!command->is_visible()) {
						command->show_all();
						auto *txt = command->get_children()[0];
						txt->grab_focus();
					} else {
						command->hide();
					}
				}
				return true;
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

// editor file tree view
namespace lith {
	namespace ui {
		namespace editor {

			treeView::treeView() {}

			treeView::~treeView() {}

		}
	}
}

// editor file tree view
namespace lith {
	namespace ui {
		namespace editor {

			commandPalette::commandPalette() : Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0) {
				auto *command_text = Gtk::manage(new Gtk::Entry());
				command_text->set_name("command");
				pack_start(*command_text, Gtk::PACK_EXPAND_WIDGET, 0);
			}

			commandPalette::~commandPalette() {}

		}
	}
}
