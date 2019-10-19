#include "editor_controller.h"

std::pair<short,short> EditorController::key_insert(const int& ch) {
	short x = view.x;
	short y = view.y;
    model.left();
    model.insert(ch);
    model.right();
    if (view.x == MAXCOLS - 1) {
        model.row_length.push(x);
        x = 0;
        y ++;
    } else {
        x ++;
    }
    return std::pair<short,short>(y,x);
}

std::pair<short,short> EditorController::key_enter() {
	short x = view.x;
	short y = view.y;
    model.row_length.push(x);
    if (model.get() != '\n') {
        model.left();
        model.insert('\n');
        model.right();
    }
    y ++;
    x = 0;
    return std::pair<short,short>(y,x);
}

std::pair<short,short> EditorController::key_backspace() {
	short x = view.x;
	short y = view.y;
    model.left();
    if (model.get() == '\t') {
        if (x > 3) {
            x -= 4;
        } else {
            int len = model.row_length.top();
            model.row_length.pop();
            y --;
            x = len - (3 - x);
        }
    } else if (x == 0 && y > 0) {
        int len = model.row_length.top();
        model.row_length.pop();
        y --;
        x = len;
    } else {
        x --;

    }
    model.del();
    model.right();
    return std::pair<short,short>(y,x);
}

std::pair<short,short> EditorController::key_tab(const int& ch) {
	short x = view.x;
	short y = view.y;
    model.left();
    model.insert(ch);
    model.right();
    if (x > MAXCOLS - 5) {
        model.row_length.push(79);
        x = x - MAXCOLS + 4;
        y ++;
    } else {
        x += 4;
    }
    return std::pair<short,short>(y,x);
}

std::pair<short,short> EditorController::key_left()  {
	short x = view.x;
	short y = view.y;
    if (x > 0) {
        model.left();
        if (model.get() == '\t') {
            if (x > 3) {
                x -= 4;
            } else if (y > 0) {
                y --;
                x = model.row_length.top() - (3 - x);
                model.row_length.pop();
            }
        } else {
            x -= 1;
        }  
        
    } else if (y > 0) {
        model.left();
        if (model.get() == '\t' && x < 4)
            x = model.row_length.top() - (3 - x);
        else
            x = model.row_length.top();
        y --;
        model.row_length.pop();
    }
    return std::pair<int,int>(y,x);
}

std::pair<short,short> EditorController::key_right()  {
	short x = view.x;
	short y = view.y;
    if (model.get() == '\0' )
            return std::pair<int,int>(y,x);

    if (model.get() == '\t') {
        if (x > MAXCOLS - 5) {
            if (y == 21)
            	return std::pair<int,int>(y,x);
            model.right();
            model.row_length.push(x);
            y ++;
            x = x - MAXCOLS + 4;
        } else {
            model.right();
            x += 4;
        }
        return std::pair<int,int>(y,x);
    }  

    if (x < MAXCOLS - 1) {

        if (model.get() == '\n') {
            if (y == 21)
        		return std::pair<int,int>(y,x);
            model.row_length.push(x);
            model.right();
            x = 0;
            y ++;
        } else {
            model.right();
            x ++ ;
        }  

    } else {
        if (y == 21)
            return std::pair<int,int>(y,x);
        model.row_length.push(x);
        model.right();
        x = 0;
        y += 1;
        return std::pair<int,int>(y,x);
    }

    return std::pair<int,int>(y,x);
}	

std::pair<short,short> EditorController::key_up() {
	short x = view.x;
	short y = view.y;
    if (y <= 0) {
		return std::pair<int,int>(y,x);
    }
    y --;
    for (unsigned short i = 0; i < MAXCOLS; ++ i) {
        model.left();
        if (model.get() == '\n') {
            i += (MAXCOLS - model.row_length.top() - 1);
            if (i >= MAXCOLS) {
                x = model.row_length.top();
                model.row_length.pop();
                return std::pair<int,int>(y,x);
            }
        } else if (model.get() == '\t') {
            i += 3;
            if (i >= MAXCOLS) {
                if (model.row_length.top() < x)
                    x = model.row_length.top();
                model.row_length.pop();
                return std::pair<int,int>(y, x - ( i + 1 - MAXCOLS ));
            }
        }
    }
    model.row_length.pop();
    return std::pair<int,int>(y,x);
}

std::pair<short,short> EditorController::key_down()  {
	short x = view.x;
	short y = view.y;
    bool newline = false;
    if (y >= 21 || view.isLastRow(model,model.start_point())) {
		return std::pair<short,short>(y,x);
    }
    y ++;
    model.row_length.push(MAXCOLS - 1);
    unsigned short count = 0;

    for (unsigned short i = 0; i < MAXCOLS; ++ i) {

        if (model.end_point() == model.get_size())
            return std::pair<short,short>(y, x + i - MAXCOLS);

        if (model.get() == '\n') {
            if (x + i < MAXCOLS) {
                model.row_length.top() =  x + i ; 
                i += MAXCOLS - (x + i) -1 ; 
                count = - 1;
                newline = true;
            }
            else {
                if (newline) 
                    return std::pair<short,short>(y,count);
                else
                    return std::pair<short,short>(y, x + i - MAXCOLS);
            }
        }
        if (model.get() == '\t') {
            i += 3;
            count += 3;
            if (i >= MAXCOLS) {
                model.right();
                if (newline)
                    return std::pair<short,short>(y, count + 1);
                return std::pair<short,short>(y, x + i - MAXCOLS + 1);
            }
        }
        model.right();
        if (model.get() == '\0')
            return std::pair<short,short>(y, x + i - MAXCOLS);

        count ++;
    }
	return std::pair<short,short>(y,x);
}

void EditorController::process_key(const int& ch) {
	std::pair<short,short> pair;
    switch (ch) {
        case 9: //tab
        	pair = key_tab(ch);
        	view.set_yx(pair);

            view.paint(model, model.start_point() - 1);
            break;
        case 127: //backspace
            if (view.x == 0 && view.y == 0)
                break;
			pair = key_backspace();
        	view.set_yx(pair);
            view.move_cursor();

            view.paint(model, model.start_point());
            break;
        case KEY_F(3): //save
            model.save();
            break;
        case 10: //enter
        	view.del_line();
        	pair = key_enter();
        	view.set_yx(pair);
            view.move_cursor();

            view.paint(model, model.start_point());
            break;
        case KEY_UP:
        	pair = key_up();
        	view.set_yx(pair);
        	break;
        case KEY_DOWN:
        	pair = key_down();
        	view.set_yx(pair);
        	break;
        case KEY_RIGHT:
        	pair = key_right();
        	view.set_yx(pair);
        	break;
        case KEY_LEFT:
        	pair = key_left();
        	view.set_yx(pair);
        	break;
        default:
        	int point = model.start_point();
        	pair = key_insert(ch);
        	view.set_yx(pair);

            view.paint(model, point);
            break;
    }

    view.draw_footer(model);
    view.refresh_all();
}