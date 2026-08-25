#include "Person.h"

// Retorna un string formateado con la información principal del paciente
// Ideal para visualizaciones compactas (como dentro de la Pila o Cola)
std::string Person::shortInfo() const {
    return nombre + " (" + tipoSangre + ", " + organo + ")";
}