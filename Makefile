CXX = gcc

CXXFLAGS = \
    -m32 \
    -ggdb3 \
    -Wall \
    -Wno-unknown-pragmas \
    -Wno-attributes \
    -fPIC \
	-DREGAMEDLL_API_ONLY \

LDFLAGS = \
    -m32 \
    -shared \
    -static-libgcc

LDLIBS = \
    -ldl \
    -lm

METAMOD = metamod-p
HLSDK = $(METAMOD)/hlsdk
REGAMEDLL = $(CURDIR)/ReGameDLL_CS

INCLUDES = \
    -I$(HLSDK)/common \
    -I$(HLSDK)/dlls \
    -I$(HLSDK)/engine \
    -I$(METAMOD)/metamod \
    -Isrc
    # -I$(REGAMEDLL)/regamedll \
    # -I$(REGAMEDLL)/regamedll/common \
    # -I$(REGAMEDLL)/regamedll/dlls \
    # -I$(REGAMEDLL)/regamedll/engine \
    # -I$(REGAMEDLL)/regamedll/public \
    # -I$(REGAMEDLL)/regamedll/public/regamedll \
    # -I$(REGAMEDLL)/regamedll/pm_shared \
    # -I$(REGAMEDLL)/regamedll/game_shared \
    # -I$(REGAMEDLL)/regamedll/dlls/API \
    # -I$(REGAMEDLL)/regamedll/dlls/bot \

TARGET = lib/test_mm_i386.so

SOURCES = \
	src/dllapi.cpp \
	src/engine_api.cpp \
	src/h_export.cpp \
	src/meta_api.cpp \
	src/sdk_util.cpp \
	src/logger.cpp \
	src/player_state.cpp \
	src/last_hope.cpp \
	src/regame/regame_api.cpp \
	src/regame/regame_hooks.cpp 


OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p lib
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

.PHONY: all clean
