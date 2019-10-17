#pragma once
#include <iostream>
#include "gap_buffer.h"
#include <stack>
#include <ncurses.h>

//max valus of cols and rows of a file
#define MAXCOLS 80
#define MAXROWS 80

class Editor{
private:

    //main buffer of the editor. Implemented as gap buffer
    GapBuffer gap_buffer;
    const char* filename;

    //stack that holds the length of the rows that are above the cursor
    std::stack<unsigned short> row_length; 

    unsigned short x; //x pos at the screen
    unsigned short y; //y pos at the screen
    size_t point; //current point of the buffer
    bool ins; //insert mode, by default = on

    //checks whether the current row is the last row of the file,
    //method uses data of gap_buffer directly, 
    bool isLastRow (const size_t& position);

    //moving the cursor up, returns the new x position
    unsigned short up(unsigned short x);

    //moving the cursor down, returns the new x position
    unsigned short down(const unsigned short& x);

public:

    //default constructor, creates a gap with size of 20
    Editor() : gap_buffer(GapBuffer()), x(0), y(0), point(0), ins(true) {
        gap_buffer.right();
    }

    //constructor from the FILE* fp, creates a gap of 20
    Editor(FILE* fp, const char* filename) : gap_buffer(GapBuffer(fp)), x(0), y(0),
    point(0), ins(true), filename(filename) {
        gap_buffer.right();
    }

    //not good method since uses data of gap_buffer directly
    void paint(const size_t& position, WINDOW* editor_window);

    //processing the input key
    void process_key(const int& ch, WINDOW* editor_window, WINDOW* footer_window);

    //initializing the ncursor window
    void initialize_window();

    //uninitializing the ncursor window
    void uninitialize_window();

    //saving the buffer into file
    void save(const char* filename);

};