CC = g++
CFLAGS = -I src/include -L src/lib
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
SRC = src/main.cpp \
      src/game.cpp \
      src/doge.cpp \
      src/land.cpp \
      src/lib.cpp \
      src/pipe.cpp \
      src/sound.cpp
OUT = main.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
