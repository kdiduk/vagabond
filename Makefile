PROJECT=fotd

CC = zcc +zx

# Origin of the game binary. Default value is 32768.
ZORG=32768

CFLAGS = -Wall -startup=1 -clib=sdcc_iy -zorg=$(ZORG) -preserve

VPATH = src
OBJDIR = obj
BINDIR = bin
INCLUDE = -Isrc
OBJECTS = $(addprefix $(OBJDIR)/, \
	display.o \
	forest.o \
	game.o \
	hud.o \
	level.o \
	main.o \
	player.o \
	titlescreen.o \
	viewport.o \
	)

$(PROJECT): prebuild $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(BINDIR)/$(PROJECT) -create-app

$(OBJECTS): $(OBJDIR)/%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

prebuild:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

.PHONY: clean
clean:
	$(RM) -rf $(OBJDIR)
	$(RM) -rf $(BINDIR)

# EOF
