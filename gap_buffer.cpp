#include "gap_buffer.h"

void GapBuffer::grow() {
    size_t new_size = (size + 1) * 2;
    char* b = new char[ new_size ];

    for (size_t i = 0; i < gap_start; ++ i)
        b[i] = data[i];

    for (size_t i = 0; i < size - gap_end; ++ i)
        b[i + new_size - (size - gap_end)] = data[gap_end + 1 + i];
    gap_end = new_size - (size - gap_end) - 1;
    size = new_size;

    char* temp = data;
    data = b;
    delete[] temp;
}

GapBuffer::GapBuffer() : gap_start(0), gap_end(19), size(20)
{
    data = new char[21];
    data[20] = '\0';
}

GapBuffer::GapBuffer(FILE* f) : gap_start(0), gap_end(19)
{
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    data = new char[fsize + 20];
    char* point = data + 20;
    fread(point, fsize, 1, f);
    point[fsize] = '\0';
    size = fsize + 20;
}

void GapBuffer::right() {
    if (gap_end != size){
        gap_end ++;
        data[gap_start] = data[gap_end];
        gap_start ++;
    }
}

void GapBuffer::left() {
    if (gap_start != 0) {
        gap_start --;
        data[gap_end] = data[gap_start];
        gap_end --;
    }
}

void GapBuffer::del(){
	if (gap_start != 0)
	gap_start --;
}

void GapBuffer::insert(const char& x) {
	if (gap_start == gap_end)
			grow();
	data[gap_start] = x;
	gap_start ++;
}

void GapBuffer::replace(const char& x) {
	if (gap_start > 0) {
		data[gap_start-1] = x;
		right();
    }
}


