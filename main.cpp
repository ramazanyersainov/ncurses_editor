#include <iostream>
#include <ncurses.h>
#include "editor_model.h"
#include "editor_view.h"
#include "editor_controller.h"
using namespace std;

inline bool file_exists(const char* name);

int main(int argc, char** argv) {

	//processing the main arguments
    if (argc < 3) {
        cout << "USAGE: [file] [mode] ('r' for reading a file; 'w' for writing a new file) " << endl;
        return 1;
    }

    string mode;

    if ((mode = argv[2]) == "w") { 
        if (file_exists(argv[1])) {
            cout << "FILE ALREADY EXISTS" << endl;
            return 1;
        } else {
            ofstream myfile;
            myfile.open(argv[1]);
            myfile << ' ';
            myfile.close();
        }
    } else if ((mode = argv[2]) == "r") {
        if (!file_exists(argv[1])) {
            cout << "FILE DOES NOT EXIST" << endl;
            return 1;
        }
    } else {
        cout << "WRONG MODE" << endl;
        return 1;
    }

    string filename = argv[1];

	EditorView view;
	set_subwindows(view);
	EditorModel model(filename);
	view.paint(model,0);
	view.refresh_all();

	EditorController controller(view,model);

	int ch;
    while ((ch = getch()) != KEY_F(1)){
        controller.process_key(ch);
    }

	return 0;
}

inline bool file_exists(const char* name) {
    if (FILE *file = fopen(name, "r")) {
        fclose(file);
        return true;
    } else {
        return false; 
    }
}
