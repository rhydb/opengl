BIN = bin
LIBS = -lm -lglfw -lGL -lglut -lGLEW
SOURCES = $(wildcard src/*.c)

OBJECTS = $(patsubst src/%.c, obj/%.o, $(SOURCES))

CFLAGS = -Isrc -std=c99

all: $(BIN)
	@echo Build complete

run: all
	./$(BIN)

$(BIN): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

obj/%.o: src/%.c src/*.h Makefile
	$(CC) -o $@ $(CFLAGS) $(LIBS) -c $<
