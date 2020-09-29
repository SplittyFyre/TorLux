all: bin/torlux
obj/main.o: src/main.cpp include/Socks.h include/Generate.h include/UI.h

	g++ -std=c++17 -O0 -MMD -Wall -c -I"include" -I"headers" src/main.cpp -o obj/main.o
obj/Socks.o: src/socks/Socks.cpp include/Socks.h include/Context.h

	g++ -std=c++17 -O0 -MMD -Wall -c -I"include" -I"headers" src/socks/Socks.cpp -o obj/Socks.o
obj/blunder.o: src/util/blunder.cpp include/blunder.h

	g++ -std=c++17 -O0 -MMD -Wall -c -I"include" -I"headers" src/util/blunder.cpp -o obj/blunder.o
obj/Generate.o: src/util/Generate.cpp include/Generate.h

	g++ -std=c++17 -O0 -MMD -Wall -c -I"include" -I"headers" src/util/Generate.cpp -o obj/Generate.o
obj/Context.o: src/chat/Context.cpp include/Context.h

	g++ -std=c++17 -O0 -MMD -Wall -c -I"include" -I"headers" src/chat/Context.cpp -o obj/Context.o
obj/UI.o: src/ui/UI.cpp include/UI.h

	g++ -std=c++17 -O0 -MMD -Wall -c -I"include" -I"headers" src/ui/UI.cpp -o obj/UI.o
bin/torlux: obj/main.o obj/Socks.o obj/blunder.o obj/Generate.o obj/Context.o obj/UI.o
	g++ -std=c++17 -O0 -MMD -Wall -L"lib" -lncurses obj/main.o obj/Socks.o obj/blunder.o obj/Generate.o obj/Context.o obj/UI.o -o bin/torlux
