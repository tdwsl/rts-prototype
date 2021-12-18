# Generated with gen_makefile.lua (tdwsl 2021)
CC = g++
OUTPUT = stipula

$(OUTPUT): obj/drawText.o obj/game.o obj/initSDL.o obj/level.o obj/loadTextures.o obj/main.o obj/map.o obj/unit.o
	$(CC) obj/* -Iinclude -o $(OUTPUT) -lSDL2

obj/drawText.o: src/drawText.cpp include/initSDL.hpp include/loadTextures.hpp include/drawText.hpp
	$(CC) -c src/drawText.cpp -Iinclude -o obj/drawText.o
obj/game.o: src/game.cpp include/initSDL.hpp include/drawText.hpp include/unit.hpp include/map.hpp include/level.hpp include/loadTextures.hpp include/game.hpp
	$(CC) -c src/game.cpp -Iinclude -o obj/game.o
obj/initSDL.o: src/initSDL.cpp include/initSDL.hpp
	$(CC) -c src/initSDL.cpp -Iinclude -o obj/initSDL.o
obj/level.o: src/level.cpp include/initSDL.hpp include/unit.hpp include/map.hpp include/level.hpp
	$(CC) -c src/level.cpp -Iinclude -o obj/level.o
obj/loadTextures.o: src/loadTextures.cpp include/loadTextures.hpp include/initSDL.hpp
	$(CC) -c src/loadTextures.cpp -Iinclude -o obj/loadTextures.o
obj/main.o: src/main.cpp include/initSDL.hpp include/loadTextures.hpp include/game.hpp
	$(CC) -c src/main.cpp -Iinclude -o obj/main.o
obj/map.o: src/map.cpp include/loadTextures.hpp include/initSDL.hpp include/map.hpp
	$(CC) -c src/map.cpp -Iinclude -o obj/map.o
obj/unit.o: src/unit.cpp include/initSDL.hpp include/loadTextures.hpp include/level.hpp include/unit.hpp
	$(CC) -c src/unit.cpp -Iinclude -o obj/unit.o

clean:
	rm obj/* $(OUTPUT)
