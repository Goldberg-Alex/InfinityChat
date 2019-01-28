CXX = clang++
CXXFLAGS = -std=c++11 -pedantic-errors -Weverything -Wno-padded -Weffc++ \
-Wno-c++98-compat -g -I$(INCLUDE) 

CPPCHECK = cppcheck 
CPPCHECKFLAGS = --enable=all --suppress=missingInclude --quiet --verbose 
			
VLG = valgrind
VLGFLAGS = --leak-check=yes --track-origins=yes --quiet

INCLUDE = "/home/student/git/include"
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
HDR = $(wildcard *.hpp)

#compile, static check, link and test
.PHONY: all
all:  test.out

.PHONY: test
test: all
	@echo "---------------Testing-----------------"
	$(VLG) $(VLGFLAGS) ./test.out
	@echo "------------Testing Done---------------"

test.out : $(OBJ) $(HDR)
	echo "---------------Checking----------------"
	$(CPPCHECK) $(CPPCHECKFLAGS) $(SRC)

	echo "---------------Linking-----------------"
	$(CXX) $(CXXFLAGS)  -o test.out $(OBJ)

%.o : %.cpp %.hpp

	echo "--------------Compiling----------------"
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDE) $*.cpp

.PHONY: clean
clean :
	rm -f *.out
	rm -f vgcore.*	
	rm -f *.o
