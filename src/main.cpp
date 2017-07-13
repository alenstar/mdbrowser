#include "globals.h"
#include "browser_wnd.h"

char master_css[] = 
{
#include "master.css.inc"
,0
};

int main (int argc, char *argv[])
{
    int iargc = 1;
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(iargc, argv, "markdown.browser");

	litehtml::context html_context;
	html_context.load_master_stylesheet(master_css);

	browser_window win(&html_context);
    if(argc > 1 && argv != NULL) {
        std::string file = std::string(argv[1]);
        win.open_url(file);
    }
	return app->run(win);
}
