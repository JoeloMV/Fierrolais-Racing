# Directorios de origen y destino
SRC_DIR := src
BIN_DIR := bin
INCLUDE_DIR := include

# Compilador y flags
CXX := g++
CXXFLAGS := -Iinclude -std=c++17
SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Obtener todos los archivos .cpp en el directorio de origen
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(CPP_FILES))

# Ejecutable principal
EXECUTABLE := $(BIN_DIR)/game.exe

# Regla por defecto
all: $(EXECUTABLE)

# Compilar el ejecutable principal
$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(SFML_LIBS)

# Compilar archivos .cpp a .o
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ejecutar el juego
run: $(EXECUTABLE)
	.\bin\game.exe

# Limpiar archivos generados
clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)

.PHONY: all run clean