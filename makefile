# Specify G++ compiler to use for compiling the code
CXX     = g++

# currently Uni linix has gcc 11 so it's support c++17 as well. 
# Specify options to pass to the compiler. Here it sets the optimisation
# level, outputs debugging info for gdb, and C++ version to use.
CXXFLAGS = -O0 -g3 -std=c++17

# if "make All" is type then ultimaltely "make all" will. this is for flexibility
All: all
# The default target to build is "GenAlloc"
all: GenAlloc

# The "GenAlloc" target depends on the object files main.o and allocation.o
GenAlloc: main.o allocation.o
	$(CXX) main.o allocation.o -o GenAlloc

# The "main.o" target depends on the source file main.cpp and the header file
main.o: main.cpp allocation.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# The "allocation.o" target depends on the source file allocation.cpp and the
allocation.o: allocation.cpp allocation.h
	$(CXX) $(CXXFLAGS) -c allocation.cpp

# The "Game.o" target depends on the source file Game.cpp and the header file
# GameTester.o: GameTester.cpp GameTester.h
# 	$(CXX) $(CXXFLAGS) -c allocation.cpp

# clean up and remove all files with this extension""~", ".o", ".stackdump", and the executable "GenAlloc"
deepclean:
	rm -f *~ *.o GenAlloc GenAlloc.exe *.stackdump

# clean up and remove all files with this extension"~", ".o", and ".stackdump" from the directory
clean:
	rm -f *~ *.o *.stackdump 

#input files
DIR = testcases/test-$*
testcase-%:
	./GenAlloc $(DIR)/1_staff.txt $(DIR)/2_project.txt $(DIR)/3_student.txt $(DIR)/4_alloc.txt 

# this command for Valgrind
VALGRIND_FULL = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

valgrind-%:
	valgrind $(VALGRIND_FULL) ./GenAlloc $(DIR)/1_staff.txt $(DIR)/2_project.txt $(DIR)/3_student.txt $(DIR)/4_alloc.txt