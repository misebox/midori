COMPILER = gcc
CFLAGS   = -g -O2 -Wall -DDEBUG
LDFLAGS  =
LIBS     =
INCLUDE  = -I./include
TARGET   = ./bin/midori
OBJDIR   = ./build
SRCDIR   = ./src
SOURCES  = $(wildcard $(SRCDIR)/*.c)
OBJECTS  = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))
DEPENDS  = $(OBJECTS:.o=.d)

TEST_COMPILER = g++
TEST_LIBS     = -lgtest_main -lgtest -lpthread
TEST_TARGET   = ./bin/test
TEST_LDFLAGS  =
TEST_SRCDIR   = ./test
TEST_SOURCES  = $(wildcard $(TEST_SRCDIR)/test_*.cpp)
TEST_OBJDIR   = ./build/test
TEST_OBJECTS  = $(addprefix $(TEST_OBJDIR)/, $(notdir $(TEST_SOURCES:.cpp=.o)))
TEST_DEPENDS  = $(TEST_OBJECTS:.o=.d)


.PHONY: $(TARGET) bin/lang

bin/lang: ./build/token.o ./build/vec.o ./build/parse.o ./build/midi.o ./build/writer.o
	gcc -o bin/lang $^ $(INCLUDE) ./src/lang.c

$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-mkdir -p $(OBJDIR)
	@[ -d $(OBJDIR) ]
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

$(TEST_OBJDIR)/%.o: $(TEST_SRCDIR)/%.cpp
	-mkdir -p $(TEST_OBJDIR)
	@[ -d $(TEST_OBJDIR) ]
	$(TEST_COMPILER) -O0 $(INCLUDE) -o $@ -c $<

$(TEST_TARGET): $(TEST_OBJECTS) ./build/midi.o ./test/main.cpp
	$(TEST_COMPILER) -o $@ $(INCLUDE) $^ $(TEST_LIBS)

test: $(TEST_TARGET)

all: clean $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEPENDS)
	rm -f $(TEST_OBJECTS) $(TEST_TARGET) $(TEST_DEPENDS)

