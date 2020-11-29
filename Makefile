BUILDDIR = build
LDFLAGS = -ljpeg -lpng
CXX = clang++
CXXFLAGS = -I. -Isrc/ -Wall -std=c++17 -g3
PLATFORM = SDL
GAPI = OPENGL

UNAME_S := $(shell uname -s)

SOURCES := $(shell find src -type f -name '*.cpp' -maxdepth 1)

ifeq ($(UNAME_S),Linux)
	SOURCES += src/platform/linux.cpp
endif

ifeq ($(PLATFORM),SDL)
	SOURCES += src/platform/sdl2.cpp
	CXXFLAGS += -DPLATFORM_SDL2
	LDFLAGS += -lSDL2
endif

ifeq ($(GAPI),OPENGL)
	SOURCES += src/gapi/opengl.cpp
	LDFLAGS += -lGL -lGLEW
	CXXFLAGS += -DGAPI_OPENGL

	ifeq ($(PLATFORM),SDL)
		SOURCES += src/gapi/opengl_sdl2.cpp
	endif
endif

OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.cpp=%.o))

BINARY = game

build: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o build/$(BINARY)

$(BUILDDIR)/%.o: %.cpp
	mkdir -p $(BUILDDIR)/$(dir $<)
	$(CXX) $(CXXFLAGS) $(IMGUI_FLAGS) -c $< -o $@

run: $(BINARY)
	./$(BUILDDIR)/$(BINARY)

clean:
	rm -rf $(BUILDDIR)
