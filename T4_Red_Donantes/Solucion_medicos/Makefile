CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2

# Lista de todos los archivos fuente
SRCS = 01_auditoria_exhaustiva.cpp \
       02_prioridad_pediatrica.cpp \
       03_cadena_donantes.cpp \
       04_busqueda_contactos.cpp \
       05_saturacion_procesador.cpp \
       06_logistica_traslado.cpp

# Reemplaza la extensión .cpp por nada para obtener los nombres de los ejecutables
EXECS = $(SRCS:.cpp=)

# Regla por defecto: compila todos los ejecutables
all: $(EXECS)

# Regla genérica para compilar cada archivo fuente individualmente
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Regla para limpiar los ejecutables compilados
clean:
	rm -f $(EXECS)

.PHONY: all clean
