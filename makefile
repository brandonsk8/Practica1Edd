# ========= Proyecto Totito Chino / Practica 1 EDD =========
# Funciona en MSYS2 MinGW 64-bit, Linux y macOS
# Compila a bin/totito(.exe)

CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -Wpedantic -O2 -MMD -MP
LDFLAGS   :=
LDLIBS    :=

SRC_DIR   := src
BIN_DIR   := bin

EXEEXT :=
ifeq ($(OS),Windows_NT)
  EXEEXT := .exe
endif

TARGET    := $(BIN_DIR)/totito$(EXEEXT)

SRCS := \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/PowerUp.cpp \
	$(SRC_DIR)/PilaPowerUps.cpp \
	$(SRC_DIR)/Jugador.cpp \
	$(SRC_DIR)/ColaTurnos.cpp \
	$(SRC_DIR)/Tablero.cpp

OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all prep clean deepclean run debug rebuild print

all: prep $(TARGET)

prep:
ifeq ($(OS),Windows_NT)
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
else
	@mkdir -p $(BIN_DIR)
endif

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	@echo ">> Ejecutando $(TARGET)"
	./$(TARGET)

debug: CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -O0 -MMD -MP
debug: clean all

rebuild: clean all

print:
	@echo "CXX      = $(CXX)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "TARGET   = $(TARGET)"
	@echo "SRCS     = $(SRCS)"

clean:
	@echo ">> Limpiando objetos y dependencias"
ifeq ($(OS),Windows_NT)
	@if exist "$(SRC_DIR)\*.o" del /q "$(SRC_DIR)\*.o"
	@if exist "$(SRC_DIR)\*.d" del /q "$(SRC_DIR)\*.d"
else
	@rm -f $(SRC_DIR)/*.o $(SRC_DIR)/*.d
endif

deepclean: clean
	@echo ">> Eliminando ejecutable"
ifeq ($(OS),Windows_NT)
	@if exist "$(TARGET)" del /q "$(TARGET)"
else
	@rm -f $(TARGET)
endif

-include $(DEPS)
