
#ifndef LITHIUM_LITHIUM_H_
#define LITHIUM_LITHIUM_H_

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/textview.h>
#include <gtkmm/treeview.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/application.h>
#include <glibmm.h>

#include <vector>
#include <fstream>

namespace lith {

	// namespace for lithium io
	namespace io {
		class sock {
		public:
			sock();
			virtual ~sock();
		protected:
			// socket info
		};
	}

	// namespace for lithium ui
	namespace ui {

		class window : public Gtk::Window {
		public:
			window();
			virtual ~window();
			void load_style(std::basic_string<char>);
		};

		// namespace for editor specific classes.
		namespace editor {

			// inherits from treeview
			class treeView : public Gtk::ScrolledWindow {
			public:
				treeView();
				virtual ~treeView();
			protected:
				Gtk::TreeView tree;

				class ModelColumns : public Gtk::TreeModel::ColumnRecord {
				public:
					ModelColumns() {
						add(m_col_name);
					}

					Gtk::TreeModelColumn<Glib::ustring> m_col_name;
				};

				ModelColumns columns;
			};

			// inherits from textview
			class textView : public Gtk::TextView {
			public:
				textView();
				void from_file(std::basic_string<char>);
				void save(std::basic_string<char>);
				virtual ~textView();
			protected:
				std::basic_string<char> file_name; // file name
			};

			class commandPalette : public Gtk::Box {
			public:
				commandPalette();
				void connect(std::basic_string<char> str, sigc::slot<void, std::basic_string<char>> func);
				void get_input();
				virtual ~commandPalette();
			protected:
				Gtk::Entry *command;
				void on_enter();

				// command registry
				typedef struct {
					std::basic_string<char> str;
					sigc::slot<void, std::basic_string<char>> func;
				} command_func;

				std::vector<command_func *> commands;
			};

			class window : public lith::ui::window {
			public:
				window();
				virtual ~window();
			protected:
				// Signal handlers
				bool on_key_press(GdkEventKey *);
				bool on_key_release(GdkEventKey *);
				void on_open_click();

				// command handlers
				void on_open_str(std::basic_string<char>);
				void on_save_str(std::basic_string<char>);

				void on_quit_click();

				// editor view
				textView *editor;
				treeView *fview;
				commandPalette *command;
			};
		}
	}

	// lithium application class
	class lithium {
	public:
		lithium(int argc, char **argv);
		void load_style(std::basic_string<char>);
		int run();
		virtual ~lithium();
	protected:
		Glib::RefPtr<Gtk::Application> app; // gtk application

		Glib::RefPtr<Gtk::CssProvider> css; // app css styling

		// window keeps track of its own tabs, files, sockets, etc.
		ui::editor::window window; // ui editor window reference
	};
}

#endif // LITHIUM_LITHIUM_H_
