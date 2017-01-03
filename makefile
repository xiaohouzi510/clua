all : clua luaso.so

clua : clua.cpp
	g++ -g -Wl,-E -o $@ $^ -llua -ldl

luaso.so : luaso.cpp
	g++ -g -fPIC -shared -o $@ $^ -llua

clean :
	rm -rf clua luaso.so
