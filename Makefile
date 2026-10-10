# Cross-platform Makefile for Rub-Da-Rabbit.
# Build on Linux/macOS with `make` and on Windows with `mingw32-make`.
# The Windows path works from cmd.exe (no POSIX shell required).

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
LDFLAGS  :=
LDLIBS   := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Platform-specific tools and libraries: cmd.exe uses different commands
# than POSIX shells, and MinGW needs SDL2's main/entry library.
ifeq ($(OS),Windows_NT)
  EXE            := .exe
  MKDIR           = if not exist $(patsubst %/,%,$(dir $@)) mkdir $(patsubst %/,%,$(dir $@))
  RM             := del /Q
  RMDIR          := rmdir /S /Q
  SDL2_MAIN_LIBS := -lmingw32 -lSDL2main
else
  EXE            :=
  MKDIR           = mkdir -p $(dir $@)
  RM             := rm -f
  RMDIR          := rm -rf
  SDL2_MAIN_LIBS :=
endif

TARGET   := bin/rubdarabbits$(EXE)

# cmd.exe's built-in `del` needs backslash separators in paths.
ifeq ($(OS),Windows_NT)
  TARGET_WS := $(subst /,\,$(TARGET))
else
  TARGET_WS := $(TARGET)
endif

SRCDIR   := src
BUILDDIR := build

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

.PHONY: all debug clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(MKDIR)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(SDL2_MAIN_LIBS) $(LDLIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(MKDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build: run `make clean && make debug` so object files are rebuilt
# with the debug flags (make does not recompile when only flags change).
debug: CXXFLAGS += -g -O0
debug: $(TARGET)

run: $(TARGET)
	$(TARGET)

clean:
	-$(RMDIR) $(BUILDDIR)
	-$(RM) $(TARGET_WS)
