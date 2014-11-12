#include "lithium.h"
#include <gtkmm/menubar.h>
#include <gtkmm/menu.h>
#include <gtkmm/textview.h>
#include <gtkmm/treestore.h>
#include <gtkmm/entry.h>
#include <iostream>
#include <fstream>

// lithium application
namespace lith {

	// application setup
	lithium::lithium(int argc, char **argv) {
		app = Gtk::Application::create(argc, argv, "org.byteflame.lithium");
		lith::ui::editor::window window();
		load_style("style/lithium.css");
	}

	void lithium::load_style(std::basic_string<char> path) {
		css = Gtk::CssProvider::get_default(); // get gtk default CssProvider
		css->load_from_path(path);
		auto screen = Gdk::Screen::get_default();
		auto ctx = Gtk::StyleContext::create();
		ctx->add_provider_for_screen(screen, css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	}

	int lithium::run() {
		return app->run(window);
	}

	lithium::~lithium() {

	}
}

// window
namespace lith {
	namespace ui {

		// creates a new button with label "Hello World".
		window::window() {}

		window::~window(){}
	}
}


// editor Window
namespace lith {
	namespace ui {
		namespace editor {

			// creates a new button with label "Hello World".
			window::window() {

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
				command->connect("open", sigc::mem_fun(*this, &window::on_open_str));
				command->connect("save", sigc::mem_fun(*this, &window::on_save_str));

				// horizontal packing box
				auto *hbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
				vbox->pack_end(*hbox, Gtk::PACK_EXPAND_WIDGET, 0);
				hbox->show();

				// add treeview
				fview = Gtk::manage(new treeView());
				fview->set_size_request(200, 0);
				hbox->pack_start(*fview, Gtk::PACK_SHRINK, 0);


				// add editor in scrolledwindow
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

			void window::on_open_str(std::basic_string<char> str) {
				editor->from_file(str);
			}

			void window::on_save_str(std::basic_string<char> str) {
				editor->save(str);
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
				} else if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_backslash) {
					if (!fview->is_visible()) {
						fview->show_all();
					} else {
						fview->hide();
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

			void textView::from_file(std::basic_string<char> str) {
				file_name = str;
				std::ifstream file(str, std::ios::in | std::ios::binary | std::ios::ate);
				if (file.is_open()) {
					auto size = file.tellg();
					auto memblock = new char [size];
					file.seekg (0, std::ios::beg);
					file.read(memblock, size);
					get_buffer()->set_text(Glib::convert_with_fallback(memblock, "UTF-8", "ISO-8859-1"));
				}
				file.close();
			}

			void textView::save(std::basic_string<char> str) {
				auto txt = get_buffer()->get_text();
				std::ofstream file(file_name);
				if (file.is_open()) {
					std::cout << txt.length() << txt.c_str() << file.write(txt.c_str(), txt.length()) << std::endl;
					file.close();
				}

			}

			textView::~textView() {}

		}
	}
}

// editor file tree view
namespace lith {
	namespace ui {
		namespace editor {

			treeView::treeView() {
				add(tree);
				set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

				auto treeModel = Gtk::TreeStore::create(columns);
				tree.set_model(treeModel);

				tree.set_reorderable();

				//Fill the TreeView's model
				Gtk::TreeModel::Row row = *(treeModel->append());
				row[columns.m_col_name] = "Billy Bob";

				Gtk::TreeModel::Row childrow = *(treeModel->append(row.children()));
				childrow[columns.m_col_name] = "Billy Bob Junior";

				childrow = *(treeModel->append(row.children()));
				childrow[columns.m_col_name] = "Sue Bob";

				row = *(treeModel->append());
				row[columns.m_col_name] = "Joey Jojo";


				row = *(treeModel->append());
				row[columns.m_col_name] = "Rob McRoberts";

				childrow = *(treeModel->append(row.children()));
				childrow[columns.m_col_name] = "Xavier McRoberts";

				//Add the TreeView's view columns:
				tree.append_column("files", columns.m_col_name);

				tree.set_name("file_view");
			}

			treeView::~treeView() {}

		}
	}
}

// command palette
namespace lith {
	namespace ui {
		namespace editor {

			commandPalette::commandPalette() : Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0) {
				command = Gtk::manage(new Gtk::Entry());
				command->set_name("command");
				command->set_placeholder_text("command, eg: 'open file.txt'");
				command->signal_activate().connect(sigc::mem_fun(*this, &commandPalette::on_enter));
				pack_start(*command, Gtk::PACK_EXPAND_WIDGET, 0);
			}

			commandPalette::~commandPalette() {}

			// registers a function for calling when a string (command) is entered
			void commandPalette::connect(std::basic_string<char> str, sigc::slot<void, std::basic_string<char>> func) {
				auto *fun = new command_func;
				fun->str = str;
				fun->func = func;
				commands.push_back(fun);
			}

			// use as general use input.
			void commandPalette::get_input() {}

			void commandPalette::on_enter() {
				// get command buffer
				auto buf = command->get_buffer();
				auto txt = buf->get_text();
				int pos = txt.find(" ");
				auto command = txt.substr(0, pos);
				auto args = txt.substr(pos + 1);

				for (auto it = commands.begin(); it != commands.end(); ++it) {
					if (command.compare((*it)->str) == 0) {
						std::cout << "got: "<< txt << " = " << (*it)->str << std::endl;
						((*it)->func)(args);
					}
				}

				// print command
				std::cout << "got: "<< buf->get_text() << std::endl;

				// clear buffer
				buf->set_text("");
			}

		}
	}
}

// editor file tree view
namespace lith {
	namespace io {

			sock::sock() {

			}

			sock::~sock() {}

	}
}
