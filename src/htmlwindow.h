#pragma once
#include <gtksourceviewmm.h>
#include "htmlwidget.h"

class HtmlWindow: public Gtk::Window
{
public:
	HtmlWindow(litehtml::context* html_context);
	virtual ~HtmlWindow();

	void open_url(const litehtml::tstring& url);
	void set_url(const litehtml::tstring& url);

private:
    void on_go_clicked();
    bool on_address_key_press(GdkEventKey* event);
    void on_file_open();
    void on_file_exit();
	void on_file_save();
	void on_file_save_as();

protected:
	HtmlWidget			m_html;
	Gtk::Entry			m_address_bar;
	Gtk::Button			m_go_button;

	Gtk::VBox			m_vbox;
	Gtk::HBox			m_hbox;

	Gtk::MenuBar 		m_menubar;

	Gtk::Menu 			m_submenu_file;
	Gtk::MenuItem		m_menuitem_file;

	Gtk::MenuItem		m_menuitem_open;
	Gtk::MenuItem		m_menuitem_exit;
	Gtk::MenuItem		m_menuitem_save;
	Gtk::MenuItem		m_menuitem_save_as;

	Gtk::Menu 			m_submenu_help;
	Gtk::MenuItem		m_menuitem_help;
	
	Gtk::MenuItem		m_menuitem_about;
	Gtk::MenuItem		m_menuitem_doc;

	Gtk::Menu 			m_submenu_view;
	Gtk::MenuItem		m_menuitem_view;
	
	Gtk::MenuItem		m_menuitem_view_md; // Markdown edit
	Gtk::MenuItem		m_menuitem_view_render; // html render
	Gtk::MenuItem		m_menuitem_view_live; // Markdown edit and render

	Gtk::ScrolledWindow m_scrolled_md;
	Gtk::ScrolledWindow m_scrolled_sv;

	Gsv::View 			m_source_view ;
	Glib::RefPtr<Gsv::Buffer> 		m_buffer;
};

