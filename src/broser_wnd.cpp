#include "logdef.h"
#include "globals.h"
#include "browser_wnd.h"
#include <gdk/gdkkeysyms.h>

browser_window::browser_window(litehtml::context* html_context) : m_html(html_context, this)//, m_go_button("_Go", true)
{
	set_title("Markdown browser");

	add(m_vbox);
	m_vbox.show();

	m_menuitem_open.set_label("Open");
	m_submenu_file.append(m_menuitem_open);
	m_menuitem_save.set_label("Save");
	m_submenu_file.append(m_menuitem_save);
	m_menuitem_save_as.set_label("Save As ...");
	m_submenu_file.append(m_menuitem_save_as);
	m_menuitem_exit.set_label("Exit");
	m_submenu_file.append(m_menuitem_exit);

	m_menuitem_file.set_label("File");
	m_menuitem_file.set_submenu(m_submenu_file);
	m_menubar.append(m_menuitem_file);

	m_menuitem_doc.set_label("Documents");
	m_submenu_help.append(m_menuitem_doc);
	m_menuitem_about.set_label("About");
	m_submenu_help.append(m_menuitem_about);

	m_menuitem_help.set_label("Help");
	m_menuitem_help.set_submenu(m_submenu_help);
	m_menubar.append(m_menuitem_help);

	m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);
	m_menubar.show_all();

	//m_vbox.pack_start(m_hbox, Gtk::PACK_SHRINK);
	//m_hbox.show();

	//m_hbox.pack_start(m_address_bar, Gtk::PACK_EXPAND_WIDGET);
	//m_address_bar.show();
	//m_address_bar.set_text((argc > 1 && argv != NULL) ? argv[1]:"../README.md");

	//m_address_bar.add_events(Gdk::KEY_PRESS_MASK);
	//m_address_bar.signal_key_press_event().connect( sigc::mem_fun(*this, &browser_window::on_address_key_press), false );

	//m_go_button.signal_clicked().connect( sigc::mem_fun(*this, &browser_window::on_go_clicked) );

	//m_hbox.pack_start(m_go_button, Gtk::PACK_SHRINK);
	//m_go_button.show();

	m_vbox.pack_start(m_scrolled_wnd, Gtk::PACK_EXPAND_WIDGET);
	m_scrolled_wnd.show();

	m_scrolled_wnd.add(m_html);
	m_html.show();

    set_default_size(800, 600);
}

browser_window::~browser_window()
{

}

void browser_window::on_go_clicked()
{
	litehtml::tstring url = m_address_bar.get_text();
	m_html.open_page(url);
}

bool browser_window::on_address_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_Return)
	{
		m_address_bar.select_region(0, -1);
		on_go_clicked();
		return true;
	}

	return false;
}

void browser_window::open_url(const litehtml::tstring &url)
{
	// m_address_bar.set_text(url);
	m_html.open_page(url);
}

void browser_window::set_url(const litehtml::tstring &url)
{
	m_address_bar.set_text(url);
}
