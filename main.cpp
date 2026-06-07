#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_PNG_Image.H> // Necesario para cargar el icono PNG
#include <iostream>
#include <string>

Fl_Text_Buffer *text_buffer = nullptr;
Fl_Button *save_btn = nullptr;
Fl_Window *window = nullptr;
std::string file_path = "";

void update_title() {
    if (!file_path.empty()) {
        size_t last_slash = file_path.find_last_of("/\\");
        std::string name = (last_slash == std::string::npos) ? file_path : file_path.substr(last_slash + 1);
        window->copy_label(("M-Pad — " + name).c_str());
    } else {
        window->label("M-Pad");
    }
}

void reset_btn_cb(void*) {
    save_btn->label("Guardar");
}

void save_cb(Fl_Widget*, void*) {
    if (file_path.empty()) {
        char *selected = fl_file_chooser("Guardar archivo", "Archivos de texto (*.txt)", NULL);
        if (selected) {
            file_path = selected;
            update_title();
        }
    }
    if (!file_path.empty()) {
        if (text_buffer->savefile(file_path.c_str()) == 0) {
            save_btn->label("¡Guardado!");
            Fl::add_timeout(1.2, reset_btn_cb);
        }
    }
}

void open_cb(Fl_Widget*, void*) {
    char *selected = fl_file_chooser("Abrir archivo", "Archivos de texto (*.txt)", NULL);
    if (selected) {
        file_path = selected;
        text_buffer->loadfile(file_path.c_str());
        update_title();
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        std::string arg1 = argv[1];
        if (arg1 == "--help" || arg1 == "-h") {
            std::cout << "\nMonojo-Pad (Edición FLTK Ultra-Ligera < 10MB)\n\n"
            << "Uso:\n  ./mpad [ruta_del_archivo]\n\n"
            << "Atajos de teclado:\n"
            << "  Control + S    Guardar el archivo.\n"
            << "  Control + O    Abrir un archivo.\n"
            << "  Control + Z    Deshacer.\n\n";
            return 0;
        }
        file_path = argv[1];
    }

    window = new Fl_Window(550, 400, "Monojo Pad");

    // --- CONFIGURACIÓN DE WM_CLASS ---
    // En FLTK, xclass define la propiedad WM_CLASS (res_class) de la ventana en X11/Wayland
    window->xclass("mpad_main");

    // --- CONFIGURACIÓN DEL ICONO ---
    // Intentar cargar el icono desde la ruta absoluta solicitada
    Fl_PNG_Image *icon = new Fl_PNG_Image("/usr/share/icons/Monojo/mpad.png");
    if (icon && icon->ld() >= 0) { // Comprobar si la imagen se cargó correctamente
        window->icon(icon);
    }

    text_buffer = new Fl_Text_Buffer();
    Fl_Text_Editor *editor = new Fl_Text_Editor(5, 5, 540, 345);
    editor->buffer(text_buffer);
    editor->textfont(FL_COURIER);

    save_btn = new Fl_Button(5, 355, 265, 35, "Guardar");
    save_btn->callback(save_cb);
    save_btn->shortcut(FL_CTRL | 's');

    Fl_Button *open_btn = new Fl_Button(280, 355, 265, 35, "Abrir");
    open_btn->callback(open_cb);
    open_btn->shortcut(FL_CTRL | 'o');

    window->resizable(editor);
    window->end();

    if (!file_path.empty()) {
        text_buffer->loadfile(file_path.c_str());
        update_title();
    }

    window->show(argc, argv);
    return Fl::run();
}
