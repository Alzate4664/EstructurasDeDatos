/*
 * ============================================================================
 * Código de Aplicación / Estructura de Datos
 * 
 * Autor: Prof. Edison Valencia Díaz
 * Perfil: https://www.eafit.edu.co/nuestros-profesores/edison-valencia-diaz
 * Curso: Estructuras de Datos y Algoritmos (EDA)
 * Institución: Universidad EAFIT
 * ============================================================================
 * Este código forma parte de las soluciones algorítmicas y estructuras de datos
 * desarrolladas para la resolución de casos de estudio hospitalarios (T4).
 * Contiene comentarios detallados sobre la complejidad y el funcionamiento.
 * ============================================================================
 */
#ifndef PERSON_H
#define PERSON_H
#include <string>
#include <vector>

/*
 * DEFINICIÓN: Struct Person (Nodo de Datos)
 * Representa la unidad de información (Payload) que viajará a través de todas las estructuras de datos.
 * Incluye tipos básicos y una estructura dinámica (std::vector) para sus compatibilidades.
 */
struct Person {
    std::string nombre;         // Nombre del paciente
    int edad;                   // Criterio numérico (usado para ordenar en el Árbol)
    std::string tipoSangre;     // Grupo sanguíneo
    std::string organo;         // Órgano requerido/donado
    std::vector<std::string> compatibles; // Arreglo dinámico de pacientes compatibles

    // Función auxiliar para obtener un resumen rápido en texto plano
    std::string shortInfo() const;
};
#endif