GXX = gcc

#Path to .h files
IDIR = include

#Path to .c files
SRC_DIR = src

#Path to .o files
OBJ_DIR = $(SRC_DIR)

#All .c files
SRC = $(wildcard $(SRC_DIR)/*.c)

#All .o files
OBJ := $(patsubst %.c,%.o,$(wildcard $(SRC_DIR)/*.c))

#All .h files
HEADERS = $(wildcard $(IDIR)/*.h)

CFLAGS = -Wall -I$(IDIR)/ -std=c++11 #c++11 is for compatibility with PSU servers
LDFLAGS = 
EXE = filter

.PHONY: all compile clean test 

#debug variable prints
#$(info Headers: $(HEADERS))
#$(info OBJ: $(OBJ))
#$(info SRC: $(SRC))
#$(info LDFLAGS: $(LDFLAGS))
#$(info CFLAGS: $(CFLAGS))



all : compile clean test

compile : $(EXE)

$(EXE) : $(OBJ)
	@$(GXX) -o $@ $^

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(HEADERS)
	@$(CXX) $(CFLAGS) -c $< -o $@

clean:
	@rm $(SRC_DIR)/*.o

test: 
	@echo "compiled"
	@echo "running code "
	@echo "--------------------"
	./$(EXE) -l Tree Boat apple
	@echo ""
	./$(EXE) -hl tree Boat apple
	@echo ""
	./$(EXE)  House -lh Boat apple
	@echo ""
	./$(EXE)  House -l Boat -h apple
	@echo ""
	./$(EXE)  House Boat apple GOAT trEe
	@echo ""
	./$(EXE)  House hat apple GOAT hat
	@echo ""

help:
	@echo "all compile clean test help"
