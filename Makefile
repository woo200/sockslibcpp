tests:
	g++ -o ./tests src/Sockets.cpp src/sockslib.cpp tests.cpp -Isrc 

dirs:
	mkdir -p obj

library: dirs
	g++ -c -o obj/sockets.o src/Sockets.cpp
	g++ -c -o obj/sockslib.o src/sockslib.cpp
	gcc obj/sockets.o obj/sockslib.o -shared -o sockslib.so 

clean: 
	rm -rf obj
	rm -f sockslib.so
	rm -f ./tests