# Compiler
CC = gcc
RC = windres

# Get GTK flags from pkg-config.
CFLAGS = $(shell pkg-config --cflags gtk+-3.0) -Wall
LIBS = $(shell pkg-config --libs gtk+-3.0)

# Executable name
TARGET = GVortex.exe

# All .c source files
SRCS = main.c app_ui.c callbacks.c downloader.c utils.c

# Obj files
OBJS = $(SRCS:.c=.o)

# Resource object file
RES_OBJ = resources.o

all: $(TARGET)

$(TARGET): $(OBJS) $(RES_OBJ)
	@echo "==> Linking executable..."
	$(CC) $(OBJS) $(RES_OBJ) -o $(TARGET) $(LIBS) -mwindows

%.o: %.c
	@echo "==> Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(RES_OBJ): resources.rc icon.ico
	@echo "==> Compiling resources..."
	$(RC) $< -o $@

clean:
	@echo "==> Cleaning up build files..."
	rm -f $(OBJS) $(RES_OBJ)

