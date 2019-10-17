#include <ncurses.h>
#include "gap_buffer.h"
#include "editor.h"
using namespace std;

inline bool file_exists(const char* name) {
    if (FILE *file = fopen(name, "r")) {
        fclose(file);
        return true;
    } else {
        return false; 
    }
}

int main(int argc, char** argv) 
{

    if (argc < 3) {
        cout << "USAGE: [file] [mode] ('r' for reading a file; 'w' for writing a new file) " << endl;
        return 1;
    }

    FILE* fp;
    string mode;

    if ((mode = argv[2]) == "w") {
        if (file_exists(argv[1])) {
            cout << "FILE ALREADY EXISTS" << endl;
            return 1;
        } else {
            fp = fopen(argv[1], "rb");
        }
    } else if ((mode = argv[2]) == "r") {
        if (file_exists(argv[1])) {
            fp = fopen(argv[1],"rb");
        } else {
            cout << "FILE DOES NOT EXIST" << endl;
            return 1;
        }
    } else {
        cout << "WRONG MODE" << endl;
        return 1;
    }
    
    Editor editor = Editor(fp);
    fclose(fp);

    editor.initialize_window();

    editor.paint(0);
    move(0,0);
    refresh();
    int ch;

    while ((ch = getch()) != KEY_F(1)){
        editor.process_key(ch);
        refresh();
    }
    editor.uninitialize_window();

    return 0;
}