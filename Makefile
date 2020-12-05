all: bin/torlux
obj/main.o: src/main.c include/tool.h

	gcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" src/main.c -o obj/main.o
obj/sender.o: src/sender.c include/tool.h

	gcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" src/sender.c -o obj/sender.o
obj/ui.o: src/ui.c

	gcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" src/ui.c -o obj/ui.o
obj/torlux.o: src/torlux.c include/torlux.h

	gcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" src/torlux.c -o obj/torlux.o
obj/tool.o: src/tool.c include/tool.h

	gcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" src/tool.c -o obj/tool.o
bin/torlux: obj/main.o obj/sender.o obj/ui.o obj/torlux.o obj/tool.o
	gcc -std=c11 -pedantic -pthread -MMD -Wall -L"lib" obj/main.o obj/sender.o obj/ui.o obj/torlux.o obj/tool.o -o bin/torlux -lncurses

.PHONY : clean
clean :
	rm -f obj/*
	rm -f bin/*
