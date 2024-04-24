CC = g++

CFLAGS = -std=c++11 -Wall

SRCDIR = .
INCDIR = -I./Network -I./Client -I./Server -I./Socket -I./Thread
BUILDDIR = build
TARGET = task


SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) $(INCDIR) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
    @mkdir -p $(BUILDDIR)
    $(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<

clean:
    rm -rf $(BUILDDIR) $(TARGET)

.PHONY: clean
