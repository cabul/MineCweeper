CC = clang
SDL = -framework SDL2 -framework SDL2_image -framework SDL2_ttf
CFLAGS = -Wall -c
LDFLAGS = $(SDL)
SOURCES = game.c
OBJECTS = $(SOURCES:.c=.o)
EXE = mine

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXE)
