/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Expresiones regulares en C++
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file html_analyzer.h
 * @brief Definición del analizador HTML mediante expresiones regulares.
 * @date 06/10/2026
 * @version 1.0
 */

#ifndef P04_HTML_ANALYZER_HTML_ANALYZER_H_
#define P04_HTML_ANALYZER_HTML_ANALYZER_H_

#include <regex>
#include <string>

#include "html_document.h"

/**
 * @brief Analiza un fichero HTML y almacena sus elementos relevantes.
 *
 * El análisis se realiza mediante expresiones regulares y se limita a las
 * etiquetas indicadas en el enunciado de la práctica.
 */
class HtmlAnalyzer {
 public:
  /**
   * @brief Construye un analizador con los patrones de la práctica.
   */
  HtmlAnalyzer();

  /**
   * @brief Analiza el fichero indicado y rellena un documento HTML.
   * @param input_filename Nombre del fichero HTML de entrada.
   * @param document Documento donde se almacenará el resultado.
   * @return true si el fichero pudo abrirse y analizarse, false si falló.
   */
  bool Analyze(const std::string& input_filename, HtmlDocument* document) const;

 private:
 // Funciones auxiliares para el análisis de HTML
  // Lee el contenido de un fichero y devuelve su contenido como cadena.
  static std::string ReadFile(const std::string& input_filename, bool* success);

  // Obtiene el número de línea correspondiente a una posición en el texto.
  static int GetLineNumber(const std::string& text, std::size_t position);

  // Comprueba si un comentario es el que actúa como descripción del documento.
  bool IsDescriptionComment(const std::string& source,
                            std::size_t doctype_end,
                            std::size_t comment_start) const;

  // Funciones de análisis de elementos HTML
  // Analiza las etiquetas HTML y las almacena en el documento.
  void AnalyzeTags(const std::string& source, HtmlDocument* document) const;

  // Analiza los comentarios HTML y los almacena en el documento.
  std::string MaskComments(const std::string& source) const;
  void AnalyzeComments(const std::string& source, HtmlDocument* document,
                       std::size_t doctype_position,
                       std::size_t doctype_length) const;

  // Analiza la declaración DOCTYPE y la almacena en el documento.
  void AnalyzeDoctype(const std::string& source, HtmlDocument* document,
                      std::size_t* position, std::size_t* length) const;

  // Expresiones regulares para el análisis de HTML                    
  std::regex tag_regex_;
  std::regex attribute_regex_;
  std::regex doctype_regex_;
  std::regex comment_regex_;
  std::regex allowed_tag_regex_;
  std::regex only_whitespace_regex_;
};

#endif