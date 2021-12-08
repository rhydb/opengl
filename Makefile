BIN = bin
LIBS = -lm -lglfw -lGL -lglut -lGLEW
SOURCES = $(wildcard src/*.c)

OBJECTS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

CFLAGS = -Isrc -std=c99

all: $(BIN)
	@echo Build complete

run: all
	./$(BIN)

$(BIN): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: src/%.c src/*.h Makefile
	$(CC) -o $@ $(CFLAGS) $(LIBS) -c $<