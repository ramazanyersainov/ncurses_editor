OBJS	= main.o gap_buffer.o editor_view.o editor_controller.o
SOURCE	= main.cpp gap_buffer.cpp editor_view.cpp editor_controller.cpp
HEADER	= gap_buffer.h editor_model.h editor_view.h editor_controller.h
OUT	= a.out
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = -lncurses

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -std=c++14

gap_buffer.o: gap_buffer.cpp
	$(CC) $(FLAGS) gap_buffer.cpp -std=c++14

editor_view.o: editor_view.cpp
	$(CC) $(FLAGS) editor_view.cpp -std=c++14

editor_controller.o: editor_controller.cpp
	$(CC) $(FLAGS) editor_controller.cpp -std=c++14


clean:
	g++ main.cpp gap_buffer.cpp editor_view.cpp -lncurses -std=c++14
