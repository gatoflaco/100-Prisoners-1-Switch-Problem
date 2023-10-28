HDR = hdr
SRC = src

ALL all: build
DEBUG debug: build-debug

CXXFLAGS := -std=c++17 -lm -pthread -lpthread -Wl,--no-as-needed
CXXFLAGS += -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy\
-Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs\
-Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion\
-Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused -O2

build: simulation
build-debug: CXXFLAGS += -g -g3
build-debug: CXXFLAGS:=$(filter-out -O2, $(CXXFLAGS))
build-debug: build

simulation: $(HDR)/* $(SRC)/*
	$(CXX) $(CXXFLAGS) -I $(HDR) -o simulation.exe $(SRC)/*.cpp

clean:
	$(RM) simulation.exe