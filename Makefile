CXX = gcc

CXXFLAGS = \
    -m32 \
    -Wall \
    -Wextra \
    -Wno-unused-parameter \
    -Wno-unknown-pragmas \
    -Wno-attributes \
    -fPIC \
    -MMD -MP

ifdef RELEASE
    CXXFLAGS += -O2
else
    CXXFLAGS += -ggdb3 -DLH_DEBUG_LOG
endif

ifeq ($(LOG_TO_CONSOLE),1)
	CXXFLAGS += -DLH_LOG_TO_CONSOLE 
endif

LDFLAGS = \
    -m32 \
    -shared \
    -static-libgcc

LDLIBS = \
    -ldl \
    -lm

METAMOD = metamod-p
HLSDK = $(METAMOD)/hlsdk

INCLUDES = \
    -I$(HLSDK)/common \
    -I$(HLSDK)/dlls \
    -I$(HLSDK)/engine \
    -I$(METAMOD)/metamod \
    -Isrc

TARGET = lib/test_mm_i386.so

MAIN_SOURCE = \
	src/main/dllapi.cpp \
	src/main/engine_api.cpp \
	src/main/h_export.cpp \
	src/main/meta_api.cpp

GAME_RULES_SOURCE = \
	src/game_rules/last_hope_rules.cpp

UTIL_SOURCE = \
	src/util/sdk_util.cpp \
	src/util/logger.cpp

CORE_SOURCE = \
	src/core/last_hope.cpp

HOOKS_SOURCE = \
	src/hooks/regame_hooks.cpp \
	src/hooks/entity_hooks.cpp \
	src/hooks/regame_loader.cpp

PLAYER_SOURCE = \
	src/player/player_team.cpp \
	src/player/player_methods.cpp \
	src/player/player_state.cpp

SOURCES = \
	$(CORE_SOURCE) \
	$(GAME_RULES_SOURCE) \
	$(HOOKS_SOURCE) \
	$(MAIN_SOURCE) \
	$(PLAYER_SOURCE) \
	$(UTIL_SOURCE)

OBJECTS = $(SOURCES:.cpp=.o)
DEPS 	= $(OBJECTS:.o=.d)

all: $(TARGET)

lib:
	mkdir -p lib

$(TARGET): $(OBJECTS) | lib
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(DEPS) 
	rm -f $(TARGET)

-include $(DEPS)

.PHONY: all clean
