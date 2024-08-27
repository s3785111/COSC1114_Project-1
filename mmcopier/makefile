PROJECT_NAME = mmcopier

BIN = bin
OBJDIR = build

# Define all source files
SRCS = $(wildcard src/*.c) 
HDRS = $(wildcard src/*.h)

# Define object files based on source files
OBJS = $(SRCS:src/%.c=build/%.o)

# Define target (executable name)
TARGET = $(BIN)/$(PROJECT_NAME)

.PHONY: all clean

all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) -o $@ $^

# Compile source files into object files
$(OBJDIR)/%.o: src/%.c $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ 

# Compile test files into object files
$(OBJDIR)/%.o: test/%.c $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -I src/

# Define clean target to remove object files and executable
clean:
	rm -rf $(OBJDIR)/* $(BIN)/*
