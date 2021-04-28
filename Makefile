CC = gcc

# define any compile-time flags
CFLAGS	:= -Wall -Wextra -g

OUTPUTDIR   := output
SRCDIR	    := src
INCLUDEDIR	:= include
OBJDIR      := obj
NAME	    := main

SOURCES = $(wildcard $(SRCDIR)/*.c)
INCLUDES = $(wildcard $(INCLUDEDIR)/*.h)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(OUTPUTDIR)/$(NAME) : $(OBJECTS)
	@$(CC) $(OBJECTS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@$(rm) $(OUTPUTDIR)/$(NAME)
	@echo "Cleanup complete!"

.PHONY: run
run:
	./$(OUTPUTDIR)/$(NAME)