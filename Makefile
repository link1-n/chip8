include ../common_flags.mk

chip8: chip8.cpp chip8.h
	$(CXX) $(CXXFLAGS) chip8.cpp -c

clean:
	rm chip8
