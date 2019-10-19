#pragma once

#include <ncurses.h>
#include <iostream>
#include "editor_model.h"

#define MAXCOLS 80
#define MAXROWS 80

class EditorView {
public:

	WINDOW* editor_window;
	WINDOW* footer_window;
	unsigned short x; //x pos at the screen
    unsigned short y; //y pos at the screen

	friend void set_subwindows(EditorView& view);
	friend class EditorController;

public:
	EditorView() : x(0), y(0)
	{
		initscr(); 
	    noecho();
	    cbreak(); 
	    keypad(stdscr, TRUE);
	    refresh();

	}

	~EditorView() {
		delwin(editor_window);
    	delwin(footer_window);
		endwin();
	}
	
	void paint(const EditorModel& model, const size_t& position);

	void refresh_all();

	void move_cursor();

	bool isLastRow (EditorModel& model, const size_t& position);

	void draw_footer(EditorModel& model);

	void del_line();

	void set_yx(std::pair<short,short>& pair);

};

void set_subwindows(EditorView& view);
