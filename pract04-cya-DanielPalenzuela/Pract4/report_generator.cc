/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Expresiones regulares en C++
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file report_generator.cc
 * @brief Implementación de la generación del esquema de salida.
 * @date 06/10/2026
 * @version 1.0
 */


#include "report_generator.h"

#include <fstream>
#include <ostream>

/**
 * @brief Escribe el esquema del documento HTML en un fichero.
 * @param output_filename Nombre del fichero de salida.
 * @param document Documento HTML ya analizado.
 * @return true si el fichero pudo generarse, false en caso contrario.
 */
bool ReportGenerator::Generate(const std::string& output_filename,
                               const HtmlDocument& document) const {
  // Abre el fichero de salida para escritura
  std::ofstream output_file(output_filename);

  // Comprueba si el fichero se abrió correctamente
  if (!output_file.is_open()) {
    return false;
  }

  // Escribe la información del documento en el fichero de salida
  output_file << "PROGRAM : " << document.GetProgramName() << '\n';
  output_file << '\n';

  output_file << "DESCRIPTION :\n";

  // Si existe una descripción, se escribe en el fichero de salida
  if (!document.GetDescription().empty()) {
    output_file << document.GetDescription() << '\n';
  }

  output_file << '\n';

  // Escribe la estructura del documento, las etiquetas, los atributos y los comentarios
  WriteStructure(&output_file, document);

  output_file << '\n';

  // Escribe las etiquetas encontradas en el documento
  WriteTags(&output_file, document);

  output_file << '\n';

  // Escribe los atributos de las etiquetas encontradas en el documento
  WriteAttributes(&output_file, document);

  output_file << '\n';

  // Escribe los comentarios encontrados en el documento
  WriteComments(&output_file, document);

  return true;
}

/**
 * @brief Convierte un valor booleano a su representación en texto.
 * @param value Valor booleano a convertir.
 * @return "True" si el valor es true, "False" si es false.
 */
std::string ReportGenerator::BoolToText(bool value) {
  return value ? "True" : "False";
}

/**
 * @brief Escribe la estructura del documento HTML en el flujo de salida.
 * @param output Flujo de salida donde se escribirá la estructura.
 * @param document Documento HTML ya analizado.
 */
void ReportGenerator::WriteStructure(std::ostream* output,
                                     const HtmlDocument& document) {
  *output << "STRUCTURE :\n";

  *output << "HTML : "
          << BoolToText(document.HasTag("html"))
          << '\n';

  *output << "HEAD : "
          << BoolToText(document.HasTag("head"))
          << '\n';

  *output << "BODY : "
          << BoolToText(document.HasTag("body"))
          << '\n';

  *output << "DOCTYPE : "
          << (document.HasDoctypeHtml5() ? "HTML5" : "None")
          << '\n';
}

/**
 * @brief Escribe las etiquetas encontradas en el documento HTML en el flujo de salida.
 * @param output Flujo de salida donde se escribirán las etiquetas.
 * @param document Documento HTML ya analizado.
 */
void ReportGenerator::WriteTags(std::ostream* output,
                                const HtmlDocument& document) {
  *output << "TAGS :\n";

  for (const HtmlTag& tag : document.GetTags()) {
    *output << "[Line " << tag.line << "] ";

    if (tag.closing) {
      *output << "/";
    }

    *output << tag.name << '\n';
  }
}

/**
 * @brief Escribe los atributos de las etiquetas encontradas en el documento HTML en el flujo de salida.
 * @param output Flujo de salida donde se escribirán los atributos.
 * @param document Documento HTML ya analizado.
 */
void ReportGenerator::WriteAttributes(
    std::ostream* output,
    const HtmlDocument& document) {
  *output << "ATTRIBUTES :\n";

  for (const HtmlTagAttributes& tag :
       document.GetTagAttributes()) {
    *output << "[Line " << tag.line << "] "
            << tag.name << '\n';

    for (const HtmlAttribute& attribute :
         tag.attributes) {
      *output << attribute.name
              << " = \""
              << attribute.value
              << "\"\n";
    }

    *output << '\n';
  }
}

/**
 * @brief Escribe los comentarios encontrados en el documento HTML en el flujo de salida.
 * @param output Flujo de salida donde se escribirán los comentarios.
 * @param document Documento HTML ya analizado.
 */
void ReportGenerator::WriteComments(
    std::ostream* output,
    const HtmlDocument& document) {
  *output << "COMMENTS :\n";

  for (const HtmlComment& comment :
       document.GetComments()) {
    if (comment.start_line == comment.end_line) {
      *output << "[Line "
              << comment.start_line
              << "]";
    } else {
      *output << "[Line "
              << comment.start_line
              << " - "
              << comment.end_line
              << "]";
    }

    if (comment.description) {
      *output << " DESCRIPTION";
    }

    *output << '\n';
    *output << comment.text << '\n';
  }
}