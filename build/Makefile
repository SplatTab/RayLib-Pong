default:
	g++ ../src/game.cpp ../src/client.cpp -o pongclient.exe -O2 -Wall -Wno-missing-braces -I ../include -L ../lib -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32
	./pongclient.exe
server:
	g++ ../src/server.cpp -o pongserver.exe -O2 -Wall -Wno-missing-braces -I ../include -L ../lib -lws2_32
	./pongserver.exe