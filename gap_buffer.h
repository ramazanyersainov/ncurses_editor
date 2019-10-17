#pragma once
#include <stdio.h>

class GapBuffer {
private:

    friend class Editor;
    char* data;
    size_t gap_start;
    size_t gap_end;
    size_t size;
    
    void grow();

public:

    GapBuffer();

    GapBuffer(FILE* f);

    ~GapBuffer() { delete[] data; }

    void right();
    
    void left();

    void del();

    void insert(const char& x);

    void replace(const char& x);

    int get() { return data[gap_start - 1]; }

    size_t start_point() { return gap_start - 1; }

    size_t end_point() { return gap_end + 1; }

    size_t get_size() { return size; }

};