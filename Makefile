COMPILER = g++
CFLAGS   = -g -Wall -O2
LDFLAGS  =
LIBS     =
INCLUDE  = -I./include
TARGET   = ./bin/midori
OBJDIR   = ./build
SOURCES  = $(wildcard src/*.c)
OBJECTS  = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))

$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	@[ -d $(OBJDIR) ]
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

all: clean $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
