COMPILER=-fpermissive -Iinclude/ -std=c++20
LINKER=-Llib/ -Lbin/ -lraylib -lopengl32 -lgdi32 -lwinmm -static-libstdc++ -static-libgcc -mwindows --static

default: main-$(OS)

run: run-$(OS)

engine: src/Dorian/*.cpp
	g++ src/Dorian/*.cpp -c $(COMPILER)

main-Windows_NT: engine
	g++ src/*.cpp *.o -o bin/game.exe -D DORIAN_INITSCENE=TitleScene $(COMPILER) $(LINKER)

format: src/format.cc
	g++ src/format.cc -c $(COMPILER)

run-Windows_NT:
	cd bin && ./game.exe

test:
	g++ src/$(FILE).cpp -D TESTING -o bin/test.exe && bin/test.exe $(COMPILER)