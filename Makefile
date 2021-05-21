BUILDDIR = build
LDFLAGS = -ljpeg -lpng
CXX = clang++
CXXFLAGS = -I. -Isrc/ -Ivm_math/public/cpp -Ivm_buffers/public/cpp -Wall -std=c++17 -g3 -DVALIDATE
PLATFORM = SDL
GAPI = OPENGL

ifeq ($(PLATFORM),SDL)
	CXXFLAGS += -DPLATFORM_SDL2
	LDFLAGS += -lSDL2 -lSDL2_ttf
endif

ifeq ($(GAPI),OPENGL)
	LDFLAGS += -lGL -lGLEW
	CXXFLAGS += -DGAPI_OPENGL
endif

LIBRARY = libsdl2_shell.so

$(LIBRARY):
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -shared $(LDFLAGS) build.cpp -o build/$(LIBRARY)

run: $(LIBRARY)
	./$(BUILDDIR)/$(LIBRARY)

clean:
	rm -rf $(BUILDDIR)
