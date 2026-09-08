CC = gcc
CXX = g++

RM = rm -rf

DIR_BUILD = build

CFLAGS = -Wall -Wextra -std=c99 -g
CXXFLAGS = -Wall -Wextra -std=c++17 -g

EXAMPLES += $(DIR_BUILD)/rb_c_typed
EXAMPLES += $(DIR_BUILD)/rb_c_void
EXAMPLES += $(DIR_BUILD)/rb_c_macro
EXAMPLES += $(DIR_BUILD)/rb_cpp

PREREQ = $(DIR_BUILD)

all: $(PREREQ) $(EXAMPLES)

$(PREREQ):
	mkdir -p $(PREREQ)

$(DIR_BUILD)/%: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(DIR_BUILD)/%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	$(RM) $(DIR_BUILD)

.PHONY: all clean