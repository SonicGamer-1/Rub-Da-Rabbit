# Cross-platform Makefile for Rub-Da-Rabbit.
#
# The platform is detected automatically. Override it when needed:
#   make PLATFORM=linux
#   mingw32-make PLATFORM=windows

PLATFORM ?= auto

ifeq ($(PLATFORM),auto)
  ifeq ($(OS),Windows_NT)
    PLATFORM := windows
  else
    PLATFORM := linux
  endif
endif

ifneq ($(filter linux windows,$(PLATFORM)),$(PLATFORM))
  $(error PLATFORM must be linux or windows)
endif

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
LDLIBS   := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

ifeq ($(PLATFORM),windows)
  EXE            := .exe
  SDL2_MAIN_LIBS := -lmingw32 -lSDL2main
  MKDIR          = if not exist $(1) mkdir $(1)
  RMDIR          = if exist $(1) rmdir /S /Q $(1)
  RM_TARGET      = if exist $(subst /,\,$(TARGET)) del /Q $(subst /,\,$(TARGET))
  RUN            = $(TARGET)
else
  EXE            :=
  SDL2_MAIN_LIBS :=
  MKDIR          = mkdir -p $(1)
  RMDIR          = rm -rf $(1)
  RM_TARGET      = rm -f $(TARGET)
  RUN            = ./$(TARGET)
endif

TARGET   := bin/rubdarabbits$(EXE)
SRCDIR   := src
BUILDDIR := build

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

.PHONY: all debug clean run

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	$(CXX) $(OBJS) -o $@ $(SDL2_MAIN_LIBS) $(LDLIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin build:
	@$(call MKDIR,$@)

debug: clean
	$(MAKE) all CXXFLAGS="$(CXXFLAGS) -g -O0"

run: all
	$(RUN)

clean:
	@$(call RMDIR,$(BUILDDIR))
	@$(RM_TARGET)
