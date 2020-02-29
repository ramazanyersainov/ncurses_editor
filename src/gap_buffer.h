#pragma once
#include <stdio.h>
#include <iostream>

class GapBuffer {
protected:

    char* data; //data array

    size_t gap_start; //starting point of the gap

    size_t gap_end; //ending point of the gap

    size_t size; //the whole size of the buffer including gap
    
    void grow(); //reallocation of the array when the gap is filled

public:

    GapBuffer(); //default constructor

    GapBuffer(std::string str); //constructor from input files

    ~GapBuffer() { delete[] data; } //deconstructor, deallocating data array

    void right(); //moving the gap to one point to the right
    
    void left(); //moving the gap to one point to the right

    void del(); //deleting an item at the current position

    void insert(const char& x); //inserting an item in the gap

    void replace(const char& x); //replacing the item at the current pos

    int get() { return data[gap_start - 1]; } //get the current item

    size_t start_point() { return gap_start - 1; } //get the index of the current item

    size_t end_point() { return gap_end + 1; } //get the index of the first item after the gap

    size_t get_size() { return size; } //get the size

    void write(const std::string& filename);

};