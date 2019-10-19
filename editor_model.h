#pragma once

#include <iostream>
#include <stack>
#include <fstream>
#include "gap_buffer.h"

class EditorModel : public GapBuffer {
private:

	std::string filename;
	std::stack<unsigned short> row_length;
	friend class EditorView;
	friend class EditorController;

public:

	EditorModel(std::string filename) : GapBuffer(filename), filename(filename), row_length(std::stack<unsigned short>()) {
		right();
	}

	void save(){
		write(filename);
	}


};