UNAME=$(shell uname)
ifeq ($(UNAME), Windows)
	CROSS=i686-w64-mingw32.static-
endif

SOURCES=$(wildcard *.cc) $(wildcard gam/*.cc)
CONTENT=$(wildcard content/*.png) $(wildcard content/*.ogg) $(wildcard content/*.wav)
BUILDDIR=$(CROSS)output
OBJECTS=$(patsubst %.cc,$(BUILDDIR)/%.o,$(SOURCES))
NAME=algernon
APP_NAME=Algernon
VERSION=$(shell git describe --tags --dirty)

CC=$(CROSS)g++
LD=$(CROSS)ld
AR=$(CROSS)ar
PKG_CONFIG=$(CROSS)pkg-config
CFLAGS=-O3 --std=c++14 -Wall -Wextra -Werror -pedantic -I gam -DNDEBUG
EMFLAGS=-s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'

ifeq ($(UNAME), Windows)
	PACKAGE=$(NAME)-windows-$(VERSION).zip
	LDFLAGS=-static-libstdc++ -static-libgcc
	LDLIBS=`$(PKG_CONFIG) sdl2 SDL2_mixer SDL2_image --cflags --libs` -Wl,-Bstatic
endif
ifeq ($(UNAME), Linux)
	PACKAGE=$(APP_NAME)-linux-$(VERSION).AppImage
	LDFLAGS=-static-libstdc++ -static-libgcc
	LDLIBS=`$(PKG_CONFIG) sdl2 SDL2_mixer SDL2_image --cflags --libs` -Wl,-Bstatic
endif
ifeq ($(UNAME), Darwin)
	PACKAGE=$(NAME)-osx-$(VERSION).tgz
	LDLIBS=-framework SDL2 -framework SDL2_mixer -framework SDL2_image -rpath @executable_path/../Frameworks
	CFLAGS+=-mmacosx-version-min=10.9
endif

EXECUTABLE=$(BUILDDIR)/$(NAME)

all: $(EXECUTABLE)

echo:
	@echo "Content: $(CONTENT)"
	@echo "Sources: $(SOURCES)"
	@echo "Uname: $(UNAME)"
	@echo "Package: $(PACKAGE)"

run: $(EXECUTABLE)
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

$(BUILDDIR)/%.o: %.cc
	@mkdir -p $(BUILDDIR)/gam
	$(CC) -c $(CFLAGS) -o $@ $<

package: $(PACKAGE)

wasm: $(NAME)-$(VERSION).html

$(NAME)-osx-$(VERSION).tgz: $(APP_NAME).app
	mkdir $(NAME)
	cp -r $(APP_NAME).app $(NAME)/.
	tar zcf $@ $(NAME)
	rm -rf $(NAME)

$(NAME)-windows-$(VERSION).zip: $(EXECUTABLE) $(CONTENT)
	mkdir -p $(NAME)/content
	cp $(EXECUTABLE) $(NAME)/`basename $(EXECUTABLE)`.exe
	cp $(CONTENT) $(NAME)/content/.
	zip -r $@ $(NAME)
	rm -rf $(NAME)

$(NAME)-$(VERSION).html: $(SOURCES) $(CONTENT)
	emcc $(CFLAGS) $(EMFLAGS) -o $@ $(SOURCES) --preload-file content/

$(APP_NAME).app: $(EXECUTABLE) launcher $(CONTENT) Info.plist
	rm -rf $(APP_NAME).app
	mkdir -p $(APP_NAME).app/Contents/{MacOS,Frameworks}
	cp $(EXECUTABLE) $(APP_NAME).app/Contents/MacOS/game
	cp launcher $(APP_NAME).app/Contents/MacOS/launcher
	cp -R content $(APP_NAME).app/Contents/MacOS/content
	cp Info.plist $(APP_NAME).app/Contents/Info.plist
	cp -R /Library/Frameworks/SDL2.framework $(APP_NAME).app/Contents/Frameworks/SDL2.framework
	cp -R /Library/Frameworks/SDL2_mixer.framework $(APP_NAME).app/Contents/Frameworks/SDL2_mixer.framework
	cp -R /Library/Frameworks/SDL2_image.framework $(APP_NAME).app/Contents/Frameworks/SDL2_image.framework

$(APP_NAME)-linux-$(VERSION).AppDir: $(EXECUTABLE) $(CONTENT)
	rm -rf $@
	mkdir -p $@/usr/{bin,lib}
	mkdir -p $@/content
	cp $(EXECUTABLE) $@/usr/bin
	cp AppRun $@/.
	cp $(APP_NAME).desktop $@/.
	cp icon.png $@/.
	cp $(CONTENT) $@/content/.
	cp /usr/lib/libSDL2{,_image,_mixer}-2.0.so.0 $@/usr/lib/.

$(APP_NAME)-linux-$(VERSION).AppImage: $(APP_NAME)-linux-$(VERSION).AppDir
	appimagetool $<

clean:
	rm -rf $(BUILDDIR) *.app *.zip *.tgz *.html *.js *.data *.wasm

.PHONY: all clean run package wasm
