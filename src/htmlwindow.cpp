#include "htmlwindow.h"
#include "globals.h"
#include "logdef.h"

#include "Madown.h"
#include "TextStream.h"

#include <gdk/gdkkeysyms.h>

HtmlWindow::HtmlWindow(litehtml::context *html_context)
    : m_html(html_context, this) //, m_go_button("_Go", true)
{
    set_title("Markdown browser");

    add(m_vbox);
    m_vbox.show();

    // File SubMenu
    m_menuitem_open.set_label("Open");
    m_menuitem_open.signal_activate().connect(
        sigc::mem_fun(*this, &HtmlWindow::on_file_open));
    m_submenu_file.append(m_menuitem_open);
    m_menuitem_save.set_label("Save");
    m_menuitem_save.signal_activate().connect(
        sigc::mem_fun(*this, &HtmlWindow::on_file_save));
    m_submenu_file.append(m_menuitem_save);
    m_menuitem_save_as.set_label("Save As ...");
    m_menuitem_save_as.signal_activate().connect(
        sigc::mem_fun(*this, &HtmlWindow::on_file_save_as));
    m_submenu_file.append(m_menuitem_save_as);
    m_menuitem_exit.set_label("Exit");
    m_menuitem_exit.signal_activate().connect(
        sigc::mem_fun(*this, &HtmlWindow::on_file_exit));
    m_submenu_file.append(m_menuitem_exit);

    m_menuitem_file.set_label("File");
    m_menuitem_file.set_submenu(m_submenu_file);
    m_menubar.append(m_menuitem_file);

    // View SubMenu
    m_menuitem_view_md.set_label("Markdown");
    m_submenu_view.append(m_menuitem_view_md);
    m_menuitem_view_render.set_label("Html");
    m_submenu_view.append(m_menuitem_view_render);
    m_menuitem_view_live.set_label("Markdown & Html");
    m_submenu_view.append(m_menuitem_view_live);

    m_menuitem_view.set_label("View");
    m_menuitem_view.set_submenu(m_submenu_view);
    m_menubar.append(m_menuitem_view);
    
    // Help SubMenu
    m_menuitem_doc.set_label("Documents");
    m_submenu_help.append(m_menuitem_doc);
    m_menuitem_about.set_label("About");
    m_submenu_help.append(m_menuitem_about);

    m_menuitem_help.set_label("Help");
    m_menuitem_help.set_submenu(m_submenu_help);
    m_menubar.append(m_menuitem_help);

    // MenuBar
    m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);
    m_menubar.show_all();


    m_source_view.set_show_line_numbers(true);
    m_source_view.set_auto_indent(true);
    m_source_view.set_indent_on_tab(false);
    m_source_view.set_tab_width(4);
    m_source_view.set_indent_width(4);
    m_source_view.set_insert_spaces_instead_of_tabs(true);
    m_vbox.pack_start(m_scrolled_sv, Gtk::PACK_EXPAND_WIDGET);
    m_scrolled_sv.show();
    m_scrolled_sv.add(m_source_view);
    m_source_view.show();
    m_buffer = m_source_view.get_source_buffer () ;
    if (!m_buffer) {
        LOGE("Gsv::View::get_source_buffer () failed");
    }
/*
    m_vbox.pack_start(m_hbox, Gtk::PACK_SHRINK);
    m_hbox.show();

	m_hbox.pack_start(m_source_view, Gtk::PACK_EXPAND_WIDGET);
	m_source_view.show();
	m_buffer = m_source_view.get_source_buffer () ;
    if (!m_buffer) {
        LOGE("Gsv::View::get_source_buffer () failed");
    }
*/
    // m_hbox.pack_start(m_address_bar, Gtk::PACK_EXPAND_WIDGET);
    // m_address_bar.show();
    // m_address_bar.set_text((argc > 1 && argv != NULL) ?
    // argv[1]:"../README.md");

    // m_address_bar.add_events(Gdk::KEY_PRESS_MASK);
    // m_address_bar.signal_key_press_event().connect( sigc::mem_fun(*this,
    // &HtmlWindow::on_address_key_press), false );

    // m_go_button.signal_clicked().connect( sigc::mem_fun(*this,
    // &HtmlWindow::on_go_clicked) );

    // m_hbox.pack_start(m_go_button, Gtk::PACK_SHRINK);
    // m_go_button.show();

    m_vbox.pack_start(m_scrolled_md, Gtk::PACK_EXPAND_WIDGET);
    m_scrolled_md.show();

    m_scrolled_md.add(m_html);
    m_html.show();

    set_default_size(800, 600);
}

HtmlWindow::~HtmlWindow() {}

void HtmlWindow::on_go_clicked() {
    litehtml::tstring url = m_address_bar.get_text();
    m_html.open_page(url);
}

bool HtmlWindow::on_address_key_press(GdkEventKey *event) {
    if (event->keyval == GDK_KEY_Return) {
        m_address_bar.select_region(0, -1);
        on_go_clicked();
        return true;
    }

    return false;
}

void HtmlWindow::open_url(const litehtml::tstring &url) {
    // m_address_bar.set_text(url);
    litehtml::tstring html;
    m_html.load_text_file(url, html);
    m_buffer->set_text(html);

    {
    md::Madown madown;
    md::IO::StringStream ss(html);
    std::string h = madown.render(ss);
    m_html.open_page(h);
    }
}

void HtmlWindow::set_url(const litehtml::tstring &url) {
    m_address_bar.set_text(url);
}

void HtmlWindow::on_file_open() {
    Gtk::FileChooserDialog dialog("Please choose a file",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN);
    // Add response buttons the the dialog:
    dialog.add_button( Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL );
    dialog.add_button( Gtk::Stock::OPEN, Gtk::RESPONSE_OK );

    //Add filters, so that only certain file types can be selected:
    Glib::RefPtr<Gtk::FileFilter> filter_md = Gtk::FileFilter::create();
    filter_md->set_name( "Markdown files" );
    filter_md->add_pattern( "*.md" );
    dialog.add_filter( filter_md );

    //Show the dialog and wait for a user response:
    int result = dialog.run();
        //Handle the response:
    switch( result )
    {
        case( Gtk::RESPONSE_OK ):{
            //Notice that this is a std::string, not a Glib::ustring.
            std::string filename = dialog.get_filename(  );
            open_url(filename);
            break;
        }
        case( Gtk::RESPONSE_CANCEL ): {
            LOGD("Cancel clicked.");
            break;
        }
        default: { LOGE("Unexpected button clicked."); break; }
    }
}
void HtmlWindow::on_file_exit() { this->close() ; }
void HtmlWindow::on_file_save() { LOGD("on_file_open"); }
void HtmlWindow::on_file_save_as() { LOGD("on_file_open"); }