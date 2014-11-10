
#ifndef LITHIUM_LITHIUM_H_
#define LITHIUM_LITHIUM_H_

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/textview.h>

namespace lith {
	namespace ui {

		class window : public Gtk::Window {
		public:
			window();
			virtual ~window();
			void load_style(std::basic_string<char>);
		protected:
			Glib::RefPtr<Gtk::CssProvider> css; // css styling
		};

		// namespace for editor specific classes.
		namespace editor {

			// inherits from textview
			class textView : public Gtk::TextView {
			public:
				textView();
				virtual ~textView();
			};

			class window : public lith::ui::window {
			public:
				window(std::basic_string<char>);
				virtual ~window();
			protected:
				// Signal handlers
				void on_button_click();
				void on_quit_click();

				// editor view
				textView *editor;
			};
		}
	}
}

#endif // LITHIUM_LITHIUM_H_
