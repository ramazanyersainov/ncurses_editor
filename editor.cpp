#include <ncurses.h>
#include "editor.h"
#include "gap_buffer.h"

bool Editor::isLastRow (const size_t& position) {
    unsigned short count = 0;
    for (size_t i = position; i <= gap_buffer.get_size() && (count + x) < 80; i ++){
        if (i <= gap_buffer.start_point() || i >= gap_buffer.end_point() ) {
            count ++;
            if (gap_buffer.data[i] == '\n')
                return false;
            if (gap_buffer.data[i] == '\0')
                return true;
        }
    }
    return false;    
}

unsigned short Editor::up(unsigned short x){
    for (unsigned short i = 0; i < MAXCOLS; ++ i) {
        gap_buffer.left();
        if (gap_buffer.get() == '\n') {
            //gap_buffer.left();
            i += (MAXCOLS - row_length.top() - 1);
            if (i >= MAXCOLS) {
                x = row_length.top();
                row_length.pop();
                return x;
            }
        }
    }
    row_length.pop();
    return x;
    /*
    for (unsigned short i = 0; i < MAXCOLS; ++ i) {
        gap_buffer.left();
        if (gap_buffer.get() == '\n') {
            gap_buffer.left();
            i += (MAXCOLS - row_length.top() - 1);
            if (i >= MAXCOLS) {
                x = row_length.top();
                row_length.pop();
                return x;
            }
        }
    }
    row_length.pop();
    return x;
    */
}

unsigned short Editor::down(const unsigned short& x) {

    bool newline = false;
    row_length.push(MAXCOLS - 1);
    unsigned short count = 0;
    for (unsigned short i = 0; i < MAXCOLS; ++ i) {

        if (gap_buffer.end_point() == gap_buffer.get_size())
            return x + i - MAXCOLS;

        if (gap_buffer.get() == '\n') {
            if (x + i < MAXCOLS) {
                row_length.top() =  x + i ; 
                i += MAXCOLS - (x + i) -1 ; 
                count = - 1;
                newline = true;
                //gap_buffer.right();
            }
            else {
                //gap_buffer.left();
                if (newline) 
                    return count;
                else
                    return x + i - MAXCOLS;
            }
        }
        gap_buffer.right();
        if (gap_buffer.get() == '\0')
            return x + i - MAXCOLS;

        count ++;
    }
    return x;

    /*
    bool newline = false;
    row_length.push(MAXCOLS - 1);
    unsigned short count = 0;
    for (unsigned short i = 0; i < MAXCOLS; ++ i) {

        if (gap_buffer.end_point() == gap_buffer.get_size())
            return x + i - MAXCOLS;

        gap_buffer.right();
        if (gap_buffer.get() == '\0')
            return x + i - MAXCOLS;
        if (gap_buffer.get() == '\n') {
            if (x + i < MAXCOLS) {
                row_length.top() =  x + i ; 
                i += MAXCOLS - (x + i) - 1; 
                count = -1;
                newline = true;
                gap_buffer.right();
            }
            else {
                gap_buffer.left();
                if (newline) 
                    return count;
                else
                    return x + i - MAXCOLS;
            }
        }
        count ++;
    }
    return x;
    */
}  

void Editor::paint(const size_t& position, WINDOW* editor_window) {
    
    for (size_t i = position; i < gap_buffer.size; ++ i) {
        if (i < gap_buffer.gap_start || i > gap_buffer.gap_end) {
            waddch(editor_window,gap_buffer.data[i]);
        }
    }
    wdelch(editor_window);
    
}  

void Editor::process_key(const int& ch, WINDOW* editor_window, WINDOW* footer_window) {
    switch (ch) {
        case 127: //backspace
            if (x == 0 && y == 0)
                break;
            gap_buffer.left();
            gap_buffer.del();
            gap_buffer.right();
            if (x == 0 && y > 0) {
                int len = row_length.top();
                row_length.pop();
                y --;
                x = len;
                wmove(editor_window,y,x);
                move(y,x);
                paint(--point,editor_window);
            } else {
                x --;
                wmove(editor_window,y,x);
                move(y,x);
                paint(--point,editor_window);
            }
            break;
        case KEY_F(3): //save
            save(filename);
            break;
        case KEY_F(2): //change mode
            ins = !ins;
            break;
        case 10: //KEY_ENTER
            row_length.push(x);

            if (gap_buffer.get() != '\n') {
                gap_buffer.left();
                gap_buffer.insert('\n');
                gap_buffer.right();
            }
            for (unsigned short i = x + 1; i < MAXCOLS; ++ i)
                mvwdelch(editor_window,y,x);

            y ++;
            x = 0;

            wmove(editor_window,y,x);
            move(y,x);
            paint(++point,editor_window);

            break;
            /* replace mode
            if (x == 0)
                row_length.push(0);
            else
                row_length.push(x - 1);
            gap_buffer.replace(ch);
            for (unsigned short i = x + 1; i < MAXCOLS; ++ i)
                mvwdelch(editor_window,y,x);

            y ++;
            x = 0;
            
            wmove(editor_window,y,x);
            move(y,x);
            point ++;
            paint(point,editor_window);

            break;
            */
        case KEY_UP:
            if (y > 0) {
                x = up(x);
                point = gap_buffer.start_point();
                y -= 1;
            }
            break;
        case KEY_DOWN:
            if (y < 21 && !isLastRow(point)) {
                x = down(x);
                point = gap_buffer.start_point();
                y += 1;
            }
            break;
        case KEY_RIGHT:
            /*
            if (x < MAXCOLS) {
                if (x == MAXCOLS - 1 && gap_buffer.get() != '\n' && y < 21) {
                    row_length.push(x);
                    gap_buffer.right();
                    point++;
                    x = 0;
                    y += 1;
                    break;
                }

                if (gap_buffer.end_point() == gap_buffer.get_size())
                    break;

                gap_buffer.right();

                if (gap_buffer.get() == '\n') {
                    if (y == 21) {
                        gap_buffer.left();
                        break;
                    }
                    row_length.push(x);
                    gap_buffer.right();
                    x = 0;
                    y ++;
                    point += 2;
                } else { 
                    x ++;
                    point ++;
                }   
            }
            */
            if (gap_buffer.end_point() == gap_buffer.get_size() + 1)
                    break;

            if (x < MAXCOLS - 1) {

                if (gap_buffer.get() == '\n') {
                    if (y == 21)
                        break;
                    row_length.push(x);
                    gap_buffer.right();
                    x = 0;
                    y ++;
                    point ++;

                } else {
                    gap_buffer.right();
                    x ++ ;
                    point ++;
                }  

            } else {
                if (y == 21)
                    break;
                row_length.push(x);
                gap_buffer.right();
                point++;
                x = 0;
                y += 1;
                break;
            }

            break;
        case KEY_LEFT:
            if (x > 0) {
                x -= 1;
                point --;
                gap_buffer.left();
            } else if (y > 0) {
                x = row_length.top();
                //if (x != MAXCOLS - 1) {
                    //point --;
                    //gap_buffer.left();
                //}
                gap_buffer.left();
                point --;
                y -= 1;
                row_length.pop();
            }
            break;
        default:
            if (!ins) { //replace mode
                gap_buffer.replace(ch);
                waddch(editor_window,ch);
                if (gap_buffer.get() == '\n') {
                    gap_buffer.right();
                    if (gap_buffer.get() == '\0') {
                        gap_buffer.left();
                        gap_buffer.left();
                        break;
                    }
                    row_length.push(x);
                    x = 0;
                    y ++;
                    point += 2;
                } else if (gap_buffer.get() == '\0') {
                    gap_buffer.left();
                } else if (x == MAXCOLS - 1){
                    row_length.push(x);
                    x = 0;
                    y ++;
                    point ++;
                } else {
                    x ++;
                    point ++;
                }
            } else { //insert mode
                gap_buffer.left();
                gap_buffer.insert(ch);
                gap_buffer.right();
                if (x == MAXCOLS - 1) { //consider the last line case
                    row_length.push(x);
                    x = 0;
                    y ++;
                } else {
                    x ++;
                }
                paint( point ++ ,editor_window);
            }
            break;
    }

    mvwhline(footer_window,0,0,'-',MAXCOLS - 1); 
    std::string mod;
    for (short i = 0; i < 80; ++ i) {
        mod += " ";
    }
    mvwprintw(footer_window,1,0,"%s",mod.c_str());
    if (ins)
        mod = "INSERT";
    else
        mod = "REPLACE";
    mvwprintw(footer_window,1,0,"MODE : %s \t\t x: %d, y: %d, buffer value: %c",mod.c_str(),x,y,gap_buffer.get());
    wrefresh(footer_window);
    move(y,x);
    wmove(editor_window,y,x);
}

void Editor::initialize_window() {

    initscr(); 
    noecho();
    cbreak(); 
    keypad(stdscr, TRUE);
    refresh();

}

void Editor::uninitialize_window() {
    endwin();
}

void Editor::save(const char* filename) {
    gap_buffer.write(filename);
}
