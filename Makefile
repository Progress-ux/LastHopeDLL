CXX = gcc

CXXFLAGS = \
    -m32 \
    -ggdb3 \
    -Wall \
    -Wno-unknown-pragmas \
    -Wno-attributes \
    -fPIC \

ifdef RELEASE
    CXXFLAGS += -O2
else
    CXXFLAGS += -Og -ggdb3 -DLH_DEBUG_LOG
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

TARGET = lib/last_hope_mm_i386.so
BUILD_DIR = build

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
	src/hooks/regame_loader.cpp \
	src/hooks/regame_context.cpp

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

OBJECTS = $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS 	= $(OBJECTS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

-include $(DEPS)

.PHONY: all clean
