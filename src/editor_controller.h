#pragma once

#include <iostream>
#include "editor_view.h"
#include "editor_model.h"

class EditorController{
public:

	EditorView& view;
	EditorModel& model;

public:

	EditorController(EditorView& view, EditorModel& model) :
	view(view), model(model) {
		process_key(KEY_LEFT);
	}
	
	std::pair<short,short> key_insert(const int& ch);
	
	std::pair<short,short> key_enter();

	std::pair<short,short> key_backspace();

	std::pair<short,short> key_tab(const int& ch);

	std::pair<short,short> key_left();

	std::pair<short,short> key_right();

	std::pair<short,short> key_up();

	std::pair<short,short> key_down();

	void key_home();

	void key_end();

	void process_key(const int& ch);

};