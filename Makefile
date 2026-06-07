# Nombre del ejecutable final
TARGET = mpad

# Compilador
CXX = g++

# Banderas de optimización (O3 para velocidad/tamaño, s para eliminar símbolos de depuración)
CXXFLAGS = $(shell fltk-config --cxxflags) -O3
LDFLAGS = $(shell fltk-config --ldflags)

# Librerías necesarias para el soporte de iconos PNG (Deben ir al final del comando)
LIBS = -lfltk_images -lpng

# Archivos fuente
SRCS = main.cpp

# Regla principal (Compilar todo)
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(SRCS) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS) $(LIBS)

# Regla para limpiar los archivos compilados
clean:
	rm -f $(TARGET)

# Regla para instalar el binario en el sistema (Opcional)
install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/$(TARGET)

.PHONY: all clean install
