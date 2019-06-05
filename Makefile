# Project: Tetris

CC       = gcc
TMPDIR   = obj
MYLIB    = obj/exception.o obj/genlib.o obj/graphics.o obj/random.o obj/simpio.o obj/strlib.o obj/imgui.o
MYOBJ    = obj/main.o obj/tetris.o obj/layout.o obj/interact.o obj/flow.o obj/fileIO.o
LIBS     = -L"C:/Program Files/Dev-Cpp/MinGW32/lib" -L"C:/Program Files/Dev-Cpp/MinGW32/mingw32/lib" -static-libstdc++ -static-libgcc -mwindows
INCS     = -I"C:/Program Files/Dev-Cpp/MinGW32/include" -I"./lib/libgraphics" -I"./lib/simpleGUI"
BIN      = tetris.exe
CFLAGS   = $(INCS)
RM       = rm -f

.PHONY: all clean cleanExceptLib

all: cleanExceptLib $(BIN)

clean:
	${RM} $(BIN)
	${RM} -r $(TMPDIR)

cleanExceptLib:
	${RM} $(BIN)
	${RM} $(MYOBJ)

$(BIN): $(TMPDIR) $(MYLIB) $(MYOBJ)
	$(CC) $(MYLIB) $(MYOBJ) -o $(BIN) $(LIBS)
# 	$(BIN)

$(TMPDIR):
	mkdir $(TMPDIR)

obj/exception.o: ./lib/libgraphics/exception.c
	$(CC) -c ./lib/libgraphics/exception.c -o obj/exception.o $(CFLAGS)

obj/genlib.o: ./lib/libgraphics/genlib.c
	$(CC) -c ./lib/libgraphics/genlib.c -o obj/genlib.o $(CFLAGS)

obj/graphics.o: ./lib/libgraphics/graphics.c
	$(CC) -c ./lib/libgraphics/graphics.c -o obj/graphics.o $(CFLAGS)

obj/random.o: ./lib/libgraphics/random.c
	$(CC) -c ./lib/libgraphics/random.c -o obj/random.o $(CFLAGS)

obj/simpio.o: ./lib/libgraphics/simpio.c
	$(CC) -c ./lib/libgraphics/simpio.c -o obj/simpio.o $(CFLAGS)

obj/strlib.o: ./lib/libgraphics/strlib.c
	$(CC) -c ./lib/libgraphics/strlib.c -o obj/strlib.o $(CFLAGS)

obj/imgui.o: ./lib/simpleGUI/imgui.c
	$(CC) -c ./lib/simpleGUI/imgui.c -o obj/imgui.o $(CFLAGS)

obj/main.o: ./main.c
	$(CC) -c ./main.c -o obj/main.o $(CFLAGS)

obj/tetris.o: ./tetris.c ./tetris.h
	$(CC) -c ./tetris.c -o obj/tetris.o $(CFLAGS)

obj/layout.o: ./layout.c ./layout.h
	$(CC) -c ./layout.c -o obj/layout.o $(CFLAGS)

obj/interact.o: ./interact.c ./interact.h
	$(CC) -c ./interact.c -o obj/interact.o $(CFLAGS)

obj/flow.o: ./flow.c ./flow.h
	$(CC) -c ./flow.c -o obj/flow.o $(CFLAGS)

obj/fileIO.o: ./fileIO.c ./fileIO.h
	$(CC) -c ./fileIO.c -o obj/fileIO.o $(CFLAGS)