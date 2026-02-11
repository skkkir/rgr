CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -fPIC
LDFLAGS = -shared
LDLIBS = -ldl

# Имена библиотек
BEAUFORT_LIB = libbeaufort.so
BINARY_LIB = libbinary.so
MAGMA_LIB = libmagma.so

# Имена объектных файлов
FILE_OBJ = file.o
BEAUFORT_OBJ = beaufort.o
BINARY_OBJ = binary.o
MAGMA_OBJ = magma.o

# Главная программа
MAIN = my_rgr
MAIN_SRC = main.cpp

all: $(BEAUFORT_LIB) $(BINARY_LIB) $(MAGMA_LIB) $(MAIN)

# Библиотека Beaufort
$(BEAUFORT_LIB): $(BEAUFORT_OBJ) $(FILE_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

# Библиотека Binary
$(BINARY_LIB): $(BINARY_OBJ) $(FILE_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

# Библиотека Magma
$(MAGMA_LIB): $(MAGMA_OBJ) $(FILE_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^


$(MAIN): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDLIBS)


$(FILE_OBJ): file.cpp file.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BEAUFORT_OBJ): beaufort.cpp beaufort.h file.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BINARY_OBJ): binary.cpp binary.h file.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(MAGMA_OBJ): magma.cpp magma.h file.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<


clean:
	rm -f *.o

.PHONY: all clean
