#include "editor_view.h"

void EditorView::paint(const EditorModel& model, const size_t& position)  {
    scrollok(editor_window, FALSE);
    int y,x;
    for (size_t i = position; i < model.size; ++ i) {
        if (i < model.gap_start || i > model.gap_end) {

            if (model.data[i] == '\t') {
                waddstr(editor_window,"    ");
            } else if (model.data[i] == '\n') {
                getyx(editor_window,y,x);
                if (y == WINDOWROWS) {
                    break;
                } else {
                    waddch(editor_window,model.data[i]);
                }
            }
            else if (model.data[i] != '\0') {
                waddch(editor_window,model.data[i]);
            }

        }
    }
    
    getyx(editor_window,y,x);
    for (int i = 0; i < MAXCOLS - x; ++ i)
        wdelch(editor_window);
    if (y < WINDOWROWS) {
        move(y + 1,0);
        wmove(editor_window,y + 1,0);
        for (int i = 0; i < MAXCOLS; ++ i) {
            wdelch(editor_window);
        }
    }    
    scrollok(editor_window, TRUE);   
}

void EditorView::refresh_all() {
    wrefresh(editor_window);
    wrefresh(footer_window);
}

void EditorView::move_cursor() {
    move(y,x);
    wmove(editor_window,y,x);
}

bool EditorView::is_last_row (EditorModel& model, const size_t& position) {
    unsigned short count = 0;
    for (size_t i = position; i <= model.get_size() && (count + x) < 80; i ++){
        if (i <= model.start_point() || i >= model.end_point() ) {
            count ++;
            if (model.data[i] == '\n')
                return false;
            if (model.data[i] == '\0')
                return true;
        }
    }
    return false;    
}

void EditorView::draw_footer(EditorModel& model){
    mvwhline(footer_window,0,0,'-',MAXCOLS - 1); 
    std::string mod;
    for (short i = 0; i < 80; ++ i) {
        mod += " ";
    }
    mvwprintw(footer_window,1,0,"%s",mod.c_str());
    mvwprintw(footer_window,1,0,"F1 = EXIT.  F3 = SAVE. HOME/END = START/END OF LINE.  x: %d, y: %d, value: %c",x,y,model.get());
    wrefresh(footer_window);
    move(y,x);
    wmove(editor_window,y,x);
}

void EditorView::del_line(){
    for (unsigned short i = x; i < MAXCOLS; ++ i)
        mvwdelch(editor_window,y,x);
}

void EditorView::set_yx(std::pair<short,short>& pair) {
    y = pair.first;
    x = pair.second;
}

void EditorView::scroll_window(const int& n) {
    wscrl(editor_window, n);
    wrefresh(editor_window);
}

void set_subwindows(EditorView& view) {
    view.editor_window = newwin(22,MAXCOLS,0,0);
    view.footer_window = newwin(2,MAXCOLS,22,0);
    scrollok(view.editor_window, TRUE);
    view.refresh_all();
    view.move_cursor();
}