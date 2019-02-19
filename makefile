SERVER = server.out
CLIENT = client.out

CXX = clang++
CXXFLAGS = -std=c++11 -pedantic-errors -Wall -Wextra -Wno-padded -Weffc++ \
-Wno-c++98-compat -g -pthread -I$(HEADERS) 
DEPENDENCY_OPTIONS = -MM -std=c++11 -I$(HEADERS) 

CPPCHECK = cppcheck 
CPPCHECKFLAGS = --enable=all --suppress=missingInclude --quiet --verbose 
RM = rm -f

TEST_DIR = tests/
SRC_DIR = src/
SRC_TEST_DIR = src/tests/

HEADERS = include/
OBJ_DIR = obj/

DEPEND_DIR = depend/

HDR = $(wildcard $(HEADERS)*.hpp)

SRC := $(filter-out %_test.cpp, $(foreach d, $(SRC_DIR), $(wildcard $(d)*.cpp)))

TEST_SRC:=$(wildcard $(SRC_TEST_DIR)*.cpp)

OBJ = $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRC))
TEST_OBJ = $(patsubst $(SRC_TEST_DIR)%.cpp, $(OBJ_DIR)%.o, $(TEST_SRC))

TEST_EXEC = $(patsubst $(OBJ_DIR)%_test.o, $(TEST_DIR)%_test.out, $(TEST_OBJ))

DEPENDENCIES = $(patsubst $(SRC_DIR)%.cpp, $(DEPEND_DIR)%.d, $(SRC))

.PHONY: all
all: $(CLIENT) $(SERVER) 

.PHONY: test 
test: all $(TEST_EXEC) 

.PHONY: print 
print:
	@echo $(TEST_EXEC)
	@echo $(TEST_OBJ)

$(TEST_EXEC) : $(TEST_OBJ)
	@mkdir -p $(TEST_DIR)
	$(CPPCHECK) $(CPPCHECKFLAGS) $(TEST_SRC)

	$(CXX) $(CXXFLAGS) -o $@ $(filter-out $(OBJ_DIR)server.o $(OBJ_DIR)client.o, $(OBJ)) $(OBJ_DIR)$(@F:.out=.o)

	@echo "---------------Testing-----------------"
	$(VLG) $(VLGFLAGS) ./$@

	@echo "------------Testing Done---------------"

$(CLIENT) : $(OBJ)
	$(CPPCHECK) $(CPPCHECKFLAGS) $(SRC)

	$(CXX) $(CXXFLAGS) -o $@ $(filter-out $(OBJ_DIR)server.o, $(OBJ))

$(SERVER) : $(OBJ)
	$(CPPCHECK) $(CPPCHECKFLAGS) $(SRC)
	
	$(CXX) $(CXXFLAGS) -o $@ $(filter-out $(OBJ_DIR)client.o, $(OBJ))

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)$*.cpp -o $@

$(TEST_OBJ) : $(TEST_SRC)
	@mkdir -p $(OBJ_DIR)

	$(CXX) $(CXXFLAGS) -c $(SRC_TEST_DIR)$(@F:.o=.cpp) -o $@

# Create .d files
$(DEPEND_DIR)%.d: $(SRC_DIR)%.cpp
	@mkdir -p $(DEPEND_DIR)
	$(CXX) $(DEPENDENCY_OPTIONS) $< -MT "$(OBJ_DIR)$*.o $*.d" -MF $(DEPEND_DIR)$*.d

# Include dependencies (if there are any)
# ifneq "$(strip $(DEPENDENCIES))" ""
-include $(DEPENDENCIES)
# endif

.PHONY: clean
clean :
	$(RM) *.out 
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(TEST_DIR)
	$(RM) -r $(DEPEND_DIR)
	$(RM) vgcore.*	
	$(RM) core
