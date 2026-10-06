/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Expresiones regulares en C++
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file p04_html_analyzer.cc
 * @brief Programa principal de la Práctica 4 de Computabilidad y Algoritmia.
 * @date 06/10/2026
 * @version 1.0
 */

#include <iostream>
#include <string>

#include "html_analyzer.h"
#include "html_document.h"
#include "report_generator.h"

namespace {

/**
 * @brief Muestra el uso del programa en la salida de error.
 * @param program_name Nombre del programa.
 */
void ShowUsage(const char* program_name) {
  std::cerr << "Uso: "
            << program_name
            << " <fichero_entrada.html> <fichero_salida.txt>\n";

  std::cerr << "  <fichero_entrada.html>: "
            << "fichero HTML que se analizará.\n";

  std::cerr << "  <fichero_salida.txt>: "
            << "fichero donde se escribirá "
            << "el esquema.\n";
}

/**
 * @brief Muestra la ayuda del programa en la salida estándar.
 * @param program_name Nombre del programa.
 */
void ShowHelp(const char* program_name) {
    std::cout << "Uso: " << program_name
    << " <fichero_entrada.html> <fichero_salida.txt>\n";
    std::cout << " " << program_name << " --help\n\n";

    std::cout << "Descripción:\n";
    std::cout << " Analiza un fichero HTML mediante expresiones regulares y genera "
    << "un esquema\n";
    std::cout << " con las etiquetas permitidas, sus atributos, la estructura "
    << "básica y los comentarios.\n\n";

    std::cout << "Parámetros:\n";
    std::cout << " <fichero_entrada.html> Fichero HTML que se analizará.\n";
    std::cout << " <fichero_salida.txt> Fichero donde se escribirá el esquema.\n";
    std::cout << " --help Muestra esta ayuda.\n";
}

/**
 * @brief Obtiene el nombre base de un fichero a partir de su ruta.
 * @param filename Ruta del fichero.
 * @return Nombre base del fichero (sin directorios).
 */
std::string GetBaseName(const std::string& filename) {
  const std::size_t separator =
      filename.find_last_of("/\\");

  if (separator == std::string::npos) {
    return filename;
  }

  return filename.substr(separator + 1);
}

}

/**
 * @brief Función principal del programa.
 * @param argc Número de argumentos.
 * @param argv Vector de argumentos.
 * @return 0 si el programa finaliza correctamente, 1 en caso de error.
 */
int main(int argc, char* argv[]) {

  // Comprueba si se solicita la ayuda del programa
  if (argc == 2 && std::string(argv[1]) == "--help") {
        ShowHelp(argv[0]);
        return 0;
  }

  // Comprueba si se han proporcionado los argumentos correctos
  if (argc != 3) {
    ShowUsage(argv[0]);
    return 1;
  }

  // Obtiene los nombres de los ficheros de entrada y salida a partir de los argumentos
  const std::string input_filename = argv[1];
  const std::string output_filename = argv[2];

  // Crea un objeto HtmlDocument para almacenar el resultado del análisis
  HtmlDocument document(GetBaseName(input_filename));
  // Crea un objeto HtmlAnalyzer para realizar el análisis del fichero HTML
  HtmlAnalyzer analyzer;

  // Analiza el fichero de entrada y rellena el documento HTML
  if (!analyzer.Analyze(input_filename, &document)) {
    std::cerr << "Error: no se pudo leer el fichero de entrada '"
              << input_filename
              << "'.\n";

    return 1;
  }

  // Crea un objeto ReportGenerator para generar el fichero de salida
  ReportGenerator generator;

  // Genera el fichero de salida a partir del documento HTML analizado
  if (!generator.Generate(output_filename, document)) {
    std::cerr << "Error: no se pudo generar el fichero de salida '"
              << output_filename
              << "'.\n";

    return 1;
  }

  return 0;
}