.PHONY: all
all: fla

objects := main.o

fla: $(objects)
	$(CXX) -o $@ $(objects)

main.o: main.cpp
	$(CXX) -std=c++14 -c main.cpp

.PHONY: clean
clean:
	rm -f $(objects) fla