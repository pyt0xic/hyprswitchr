SOURCE_FILES=$(wildcard src/*.cpp)

all:
	$(CXX) -O2 -shared -fPIC --no-gnu-unique $(SOURCE_FILES)  -o hyprswitchr.so `pkg-config --cflags pixman-1 libdrm hyprland` -std=c++2b
	strip hyprswitchr.so

debug:
	$(CXX) -O2 -shared -fPIC --no-gnu-unique $(SOURCE_FILES)  -o hyprswitchr.so `pkg-config --cflags pixman-1 libdrm hyprland` -std=c++2b

clean:
	rm ./hyprswitchr.so
