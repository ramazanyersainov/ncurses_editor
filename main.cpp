#include <ncurses.h>
#include "gap_buffer.h"
#include "editor.h"
#include <fstream>
using namespace std;

//to check whether a file exists
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

    //processign the main arguments
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
            ofstream myfile;
            myfile.open(argv[1]);
            myfile << ' ';
            myfile.close();
            fp = fopen(argv[1],"rb");
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
    
    //creting an editor object

    Editor editor = Editor(fp,argv[1]);
    fclose(fp);

    editor.initialize_window(); //initialize ncurses
    WINDOW* editor_window = newwin(22,MAXCOLS,0,0);
    WINDOW* footer_window = newwin(2,MAXCOLS,22,0);
    scrollok(editor_window,TRUE);
    wscrl(editor_window,5);

    wmove(editor_window,0,0);
    move(0,0);

    editor.paint(0, editor_window); //paint the initial screen
    wmove(editor_window,0,0);
    move(0,0);
    editor.process_key(KEY_LEFT,editor_window,footer_window);
    wrefresh(editor_window);
    wrefresh(footer_window);
    int ch;

    while ((ch = getch()) != KEY_F(1)){ //main loop, processing the input key till F1 is pressed
        editor.process_key(ch,editor_window,footer_window); //processing a key
        wrefresh(editor_window);
        wrefresh(footer_window);
    }
    delwin(editor_window);
    delwin(footer_window);
    editor.uninitialize_window(); //endwin() of ncurses
    return 0;
}