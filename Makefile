# Makefile pentru Stol de Pasari - Boids
# macOS cu GLEW si GLFW instalate via Homebrew

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lglfw -lGLEW -framework OpenGL

# Directoare
SRCDIR = src
OBJDIR = obj

# Fisiere sursa
SRCS = main.cpp globals.cpp boids.cpp render.cpp input.cpp init.cpp
OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

# Numele executabilului
TARGET = stolPasari

# Regula implicita
all: $(OBJDIR) $(TARGET)

# Creare director pentru obiecte daca nu exista
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Linkare
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compilare fisiere .cpp in .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dependente header
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/config.h $(SRCDIR)/init.h $(SRCDIR)/boids.h $(SRCDIR)/render.h $(SRCDIR)/input.h
$(OBJDIR)/globals.o: $(SRCDIR)/globals.cpp $(SRCDIR)/globals.h $(SRCDIR)/pasare.h $(SRCDIR)/config.h
$(OBJDIR)/boids.o: $(SRCDIR)/boids.cpp $(SRCDIR)/boids.h $(SRCDIR)/config.h $(SRCDIR)/globals.h $(SRCDIR)/pasare.h
$(OBJDIR)/render.o: $(SRCDIR)/render.cpp $(SRCDIR)/render.h $(SRCDIR)/config.h $(SRCDIR)/globals.h $(SRCDIR)/pasare.h
$(OBJDIR)/input.o: $(SRCDIR)/input.cpp $(SRCDIR)/input.h $(SRCDIR)/config.h $(SRCDIR)/globals.h $(SRCDIR)/init.h
$(OBJDIR)/init.o: $(SRCDIR)/init.cpp $(SRCDIR)/init.h $(SRCDIR)/config.h $(SRCDIR)/globals.h $(SRCDIR)/pasare.h


# Curatare
clean:
	rm -f $(OBJDIR) $(TARGET)

# Rulare
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
