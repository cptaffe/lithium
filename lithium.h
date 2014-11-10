
#ifndef LITHIUM_LITHIUM_H_
#define LITHIUM_LITHIUM_H_

#include <gtkmm/button.h>
#include <gtkmm/window.h>

namespace lith {
	namespace ui {
		class window : public Gtk::Window {

		public:
			window(std::basic_string<char>);
			virtual ~window();
			
		protected:
			//Signal handlers:
			void on_button_clicked();

			//Member widgets:
			Gtk::Button m_button;
		};
	}
}

#endif // LITHIUM_LITHIUM_H_
