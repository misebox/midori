COMPILER = gcc
CFLAGS   = -g -Wall -O2 -DDEBUG
LDFLAGS  =
LIBS     =
INCLUDE  = -I./include
TARGET   = ./bin/midori
OBJDIR   = ./build
SRCDIR   = ./src
SOURCES  = $(wildcard $(SRCDIR)/*.c)
OBJECTS  = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))
DEPENDS  = $(OBJECTS:.o=.d)

$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-mkdir -p $(OBJDIR)
	@[ -d $(OBJDIR) ]
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

all: clean $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEPENDS)
