#include <iostream>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include "Ieee754.hpp"

int main(int argc, char **argv)
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("ieee754.glade");
    Ieee754 ieee754{builder};
    Gtk::Window *window1{nullptr};
    builder->get_widget("window1", window1);
    app->run(*window1);
    return 0;
}