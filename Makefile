all: bin/torlux
obj/main.o: src/main.cpp include/TorLux.h include/Generate.h \
 include/Context.h

	g++ -std=c++17 -O0 -pthread -MMD -Wall -c -I"include" -I"headers" src/main.cpp -o obj/main.o
obj/util.o: src/util/util.cpp include/util.h include/Socks.h \
 include/Server.h include/UI.h

	g++ -std=c++17 -O0 -pthread -MMD -Wall -c -I"include" -I"headers" src/util/util.cpp -o obj/util.o
obj/Generate.o: src/util/Generate.cpp include/Generate.h include/util.h

	g++ -std=c++17 -O0 -pthread -MMD -Wall -c -I"include" -I"headers" src/util/Generate.cpp -o obj/Generate.o
obj/TorLux.o: src/chat/TorLux.cpp include/TorLux.h include/util.h \
 include/Socks.h include/UI.h include/Sender.h include/Server.h \
 include/Context.h include/Generate.h

	g++ -std=c++17 -O0 -pthread -MMD -Wall -c -I"include" -I"headers" src/chat/TorLux.cpp -o obj/TorLux.o
obj/Context.o: src/chat/Context.cpp include/Context.h

	g++ -std=c++17 -O0 -pthread -MMD -Wall -c -I"include" -I"headers" src/chat/Context.cpp -o obj/Context.o
obj/Sender.o: src/network/Sender.cpp include/Sender.h include/Socks.h \
 include/TorLux.h

	g++ -std=c++17 -O0 -pthread -MMD -Wall -c -I"include" -I"headers" src/network/Sender.cpp -o obj/Sender.o
obj/Socks.o: src/network/Socks.cpp include/Socks.h include/Context.h \
 include/util.h

	g++ -std=c++17 -O0 -pthread -MMD -Wall -c -I"include" -I"headers" src/network/Socks.cpp -o obj/Socks.o
obj/Server.o: src/network/Server.cpp include/Server.h include/TorLux.h \
 include/Context.h include/util.h

	g++ -std=c++17 -O0 -pthread -MMD -Wall -c -I"include" -I"headers" src/network/Server.cpp -o obj/Server.o
obj/UI.o: src/ui/UI.cpp include/UI.h include/Sender.h include/TorLux.h

	g++ -std=c++17 -O0 -pthread -MMD -Wall -c -I"include" -I"headers" src/ui/UI.cpp -o obj/UI.o
bin/torlux: obj/main.o obj/util.o obj/Generate.o obj/TorLux.o obj/Context.o obj/Sender.o obj/Socks.o obj/Server.o obj/UI.o
	g++ -std=c++17 -O0 -pthread -MMD -Wall -L"lib" obj/main.o obj/util.o obj/Generate.o obj/TorLux.o obj/Context.o obj/Sender.o obj/Socks.o obj/Server.o obj/UI.o -o bin/torlux -lncurses
