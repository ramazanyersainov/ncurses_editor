#pragma once
#include <iostream>
#include "gap_buffer.h"
#include <stack>

#define MAXCOLS 80
#define MAXROWS 80

class Editor{
private:

    GapBuffer gap_buffer;
    std::stack<unsigned short> row_length;

    unsigned short x;
    unsigned short y;
    size_t point;
    bool ins; //insert mode, by default = off

    //not good method since uses data of gap_buffer directly
    bool isLastRow (const size_t& position);

    unsigned short up(unsigned short x);

    unsigned short down(const unsigned short& x);

public:
    Editor() : gap_buffer(GapBuffer()), x(0), y(0), point(0), ins(false)  {
        gap_buffer.right();
    }

    Editor(FILE* fp) : gap_buffer(GapBuffer(fp)), x(0), y(0), point(0), ins(false)  {
        gap_buffer.right();
    }

    //not good method since uses data of gap_buffer directly
    void paint(const size_t& position);

    void process_key(const int& ch);

    void initialize_window();

    void uninitialize_window();

};