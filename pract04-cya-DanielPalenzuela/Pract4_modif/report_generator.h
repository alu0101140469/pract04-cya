/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Expresiones regulares en C++
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file report_generator.h
 * @brief Definición de la generación del esquema de salida.
 * @date 06/10/2026
 * @version 1.0
 */

#ifndef P04_HTML_ANALYZER_REPORT_GENERATOR_H_
#define P04_HTML_ANALYZER_REPORT_GENERATOR_H_

#include <iosfwd>
#include <string>

#include "html_document.h"

/**
 * @brief Genera el fichero de resumen a partir del documento analizado.
 */
class ReportGenerator {
 public:
  /**
   * @brief Escribe el esquema del documento HTML en un fichero.
   * @param output_filename Nombre del fichero de salida.
   * @param document Documento HTML ya analizado.
   * @return true si el fichero pudo generarse, false en caso contrario.
   */
  bool Generate(const std::string& output_filename,
                const HtmlDocument& document,
                const std::string& input_filename) const; // modif
 private:
 // Funciones auxiliares para la generación del esquema de salida
  static std::string BoolToText(bool value);

  static void WriteStructure(std::ostream* output,
                             const HtmlDocument& document);

  static void WriteTags(std::ostream* output,
                        const HtmlDocument& document);

  static void WriteAttributes(std::ostream* output,
                              const HtmlDocument& document);

  static void WriteComments(std::ostream* output,
                            const HtmlDocument& document);
};

#endif