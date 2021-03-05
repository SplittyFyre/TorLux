all: bin/torlux
obj/torlux.o: src/torlux.c include/torlux.h include/tool.h \
  include/server.h include/sender.h include/ui.h

	gcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" src/torlux.c -o obj/torlux.o
obj/tool.o: src/tool.c include/tool.h

	gcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" src/tool.c -o obj/tool.o
obj/server.o: src/server.c include/server.h include/torlux.h \
  include/tool.h

	gcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" src/server.c -o obj/server.o
obj/main.o: src/main.c include/tool.h include/torlux.h

	gcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" src/main.c -o obj/main.o
obj/ui.o: src/ui.c include/ui.h include/torlux.h include/sender.h

	gcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" src/ui.c -o obj/ui.o
obj/sender.o: src/sender.c include/torlux.h include/tool.h

	gcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" src/sender.c -o obj/sender.o
bin/torlux: obj/torlux.o obj/tool.o obj/server.o obj/main.o obj/ui.o obj/sender.o
	gcc -std=c11 -pedantic -pthread -MMD -Wall -L"lib" obj/torlux.o obj/tool.o obj/server.o obj/main.o obj/ui.o obj/sender.o -o bin/torlux -lncurses

.PHONY : clean
clean :
	rm -f obj/*
	rm -f bin/*
