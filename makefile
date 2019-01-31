SERVER = server.out
CLIENT = client.out

CXX = clang++
CXXFLAGS = -std=c++11 -pedantic-errors -Weverything -Wno-padded -Weffc++ \
-Wno-c++98-compat -g -pthread -I$(HEADERS) 
DEPENDENCY_OPTIONS = -MM -std=c++11 -I$(HEADERS) 

CPPCHECK = cppcheck 
CPPCHECKFLAGS = --enable=all --suppress=missingInclude --quiet --verbose 
RM = rm -f

TEST_DIR = ./tests/
SRC_DIR = ./src/
SRC_TEST_DIR = ./src/tests

HEADERS = ./include/
OBJ_DIR = ./obj/

DEPEND_DIR = ./depend/

# SUBDIRS := $(shell ls -F | grep "\/" )
# DIRS := ./ $(SUBDIRS)

# SRC = client.cpp command.cpp epoll.cpp semaphore.cpp server.cpp socket.cpp user_list.cpp user.cpp 

HDR = $(wildcard $(HEADERS)*.hpp)

SRC := $(filter-out %_test.cpp, $(foreach d, $(SRC_DIR), $(wildcard $(d)*.cpp)))
# TEST_SRC:=$(filter %_test.cpp, $(foreach d, $(SRC_DIR), $(wildcard $(d)*.cpp)))

OBJ = $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRC))
# TEST_OBJ = $(patsubst $(SRC_TEST_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRC))

DEPENDENCIES = $(patsubst $(SRC_DIR)%.cpp, $(DEPEND_DIR)%.d, $(SRC))

#compile, static check, link and test
.PHONY: all
# all:  $(SERVER) $(CLIENT) $(DEPENDENCIES)
all: $(CLIENT) $(SERVER)
# .PHONY: test
# test: all
# 	@echo "---------------Testing-----------------"
# #	sudo $(VLG) $(VLGFLAGS) ./$(PROJECT)
# 	sudo ./$(PROJECT)

# 	@echo "------------Testing Done---------------"


$(CLIENT) : $(OBJ)
	echo "---------------Checking----------------"
	$(CPPCHECK) $(CPPCHECKFLAGS) $(SRC)

	echo "---------------Linking-----------------"
	$(CXX) $(CXXFLAGS) -o $@ $(filter-out $(OBJ_DIR)server.o, $(OBJ))


$(SERVER) : $(OBJ)
	echo "---------------Checking----------------"
	$(CPPCHECK) $(CPPCHECKFLAGS) $(SRC)

	echo "---------------Linking-----------------"
	$(CXX) $(CXXFLAGS) -o $@ $(filter-out $(OBJ_DIR)client.o, $(OBJ))

# $(TEST_DIR)%.out :  $(OBJ_DIR)%.o

	echo "---------------Checking----------------"
	$(CPPCHECK) $(CPPCHECKFLAGS) $(SRC)

	echo "---------------Linking-----------------"
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	echo "--------------Compiling----------------"
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)$*.cpp -o $@
	# mv $@ $(OBJ_DIR)

# Create .d files
$(DEPEND_DIR)%.d:  $(SRC_DIR)%.cpp
	$(CXX) $(DEPENDENCY_OPTIONS) $< -MT "$(OBJ_DIR)$*.o $*.d" -MF $(DEPEND_DIR)$*.d

# Include dependencies (if there are any)
ifneq "$(strip $(DEPENDENCIES))" ""
-include $(DEPENDENCIES)
endif

.PHONY: clean
clean :
	$(RM) $(PROJECT) 
	$(RM) $(OBJ)
	$(RM) vgcore.*	
	$(RM) core


# #-- reference--------------------
# # Project Name (executable)
# PROJECT = demoproject
# # Compiler
# CC = g++

# # Run Options       
# COMMANDLINE_OPTIONS = /dev/ttyS0

# # Compiler options during compilation
# COMPILE_OPTIONS = -ansi -pedantic -Wall

# #Header include directories
# HEADERS =
# #Libraries for linking
# LIBS =

# # Dependency options
# DEPENDENCY_OPTIONS = -MM

# #-- Do not edit below this line --

# # Subdirs to search for additional source files
# SUBDIRS := $(shell ls -F | grep "\/" )
# DIRS := ./ $(SUBDIRS)
# SOURCE_FILES := $(foreach d, $(DIRS), $(wildcard $(d)*.cpp) )

# # Create an object file of every cpp file
# OBJECTS = $(patsubst %.cpp, %.o, $(SOURCE_FILES))

# # Dependencies
# DEPENDENCIES = $(patsubst %.cpp, %.d, $(SOURCE_FILES))

# # Create .d files
# %.d: %.cpp
#     $(CC) $(DEPENDENCY_OPTIONS) $< -MT "$*.o $*.d" -MF $*.d

# # Make $(PROJECT) the default target
# all: $(DEPENDENCIES) $(PROJECT)

# $(PROJECT): $(OBJECTS)
#     $(CC) -o $(PROJECT) $(OBJECTS) $(LIBS)

# # Include dependencies (if there are any)
# ifneq "$(strip $(DEPENDENCIES))" ""
#   include $(DEPENDENCIES)
# endif

# # Compile every cpp file to an object
# %.o: %.cpp
#     $(CC) -c $(COMPILE_OPTIONS) -o $@ $< $(HEADERS)

# # Build & Run Project
# run: $(PROJECT)
#     ./$(PROJECT) $(COMMANDLINE_OPTIONS)

# # Clean & Debug
# .PHONY: makefile-debug
# makefile-debug:

# .PHONY: clean
# clean:
#     rm -f $(PROJECT) $(OBJECTS)

# .PHONY: depclean
# depclean:
#     rm -f $(DEPENDENCIES)
