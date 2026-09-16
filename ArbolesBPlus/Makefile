# =========================================================
# MAKEFILE AVANZADO - PROYECTO ÁRBOL B+ (EDA 2026)
# =========================================================

# Compilador y Banderas
CC      := gcc
CFLAGS  := -Wall -Wextra -Wpedantic -std=c99 -O2 -g
TARGETS := bplus_tree bplus_tree_multi

# Definición de colores para mejor experiencia en consola
CYAN    := \033[36m
GREEN   := \033[32m
YELLOW  := \033[33m
RESET   := \033[0m

# PHONY previene conflictos si llegara a existir un archivo llamado 'clean' o 'all'
.PHONY: all clean run run_multi help

# Regla por defecto (compila ambos proyectos)
all: $(TARGETS)
	@echo "$(GREEN)✔ Todos los programas compilados con éxito.$(RESET)"

# Regla genérica y dinámica (Pattern Rule)
# Esto compila automáticamente cualquier archivo .c sin tener que repetir comandos
%: %.c
	@echo "$(CYAN)⚙ Compilando $@...$(RESET)"
	@$(CC) $(CFLAGS) $< -o $@

# Regla de limpieza
clean:
	@echo "$(YELLOW)🗑 Limpiando binarios...$(RESET)"
	@rm -f $(TARGETS)
	@echo "$(GREEN)✔ Limpieza terminada.$(RESET)"

# Accesos directos de ejecución
run: bplus_tree
	@echo "$(GREEN)▶ Iniciando versión original (Global)...$(RESET)"
	@./bplus_tree

run_multi: bplus_tree_multi
	@echo "$(GREEN)▶ Iniciando versión Orientada a Objetos...$(RESET)"
	@./bplus_tree_multi

# Ayuda en consola
help:
	@echo "$(CYAN)========================================="
	@echo "   COMANDOS DISPONIBLES EN MAKE"
	@echo "=========================================$(RESET)"
	@echo "$(YELLOW)make$(RESET)             - Compila todo el proyecto."
	@echo "$(YELLOW)make run$(RESET)         - Compila y arranca la versión original."
	@echo "$(YELLOW)make run_multi$(RESET)   - Compila y arranca la versión de Múltiples Árboles."
	@echo "$(YELLOW)make clean$(RESET)       - Elimina ejecutables para recompilar de cero."
	@echo "$(YELLOW)make help$(RESET)        - Muestra este menú de ayuda."
