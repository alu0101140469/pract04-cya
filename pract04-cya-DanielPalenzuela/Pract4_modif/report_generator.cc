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
#include <iterator>
#include <regex>
#include <string>

// modif
namespace {

/**
 MODIF
 * @brief Lee un fichero de texto.
 */
std::string ReadTextFile(const std::string& filename, bool* success) {
  std::ifstream input_file(filename);

  if (!input_file.is_open()) {
    *success = false;
    return std::string();
  }

  std::string content(
      (std::istreambuf_iterator<char>(input_file)),
      std::istreambuf_iterator<char>());

  *success = true;
  return content;
}

/**
 MODIF
 * @brief Incrementa en una unidad un número representado como texto.
 */
std::string IncrementNumber(const std::string& number) {
  std::string result = number;
  int carry = 1;

  for (auto character = result.rbegin();
       character != result.rend() && carry != 0;
       ++character) {
    if (*character == '9') {
      *character = '0';
    } else {
      ++(*character);
      carry = 0;
    }
  }

  if (carry != 0) {
    result.insert(result.begin(), '1');
  }

  return result;
}

/**
 MODIF
 * @brief Cambiar todas las etiquetas hN por hN+1.
 */
std::string IncrementHeaders(const std::string& html) {
  const std::regex header_regex(
      R"(<\s*/?\s*h([0-9]+)\b[^>]*>)",
      std::regex::icase);

  std::string modified_html;
  std::size_t last_position = 0;

  std::sregex_iterator current(
      html.begin(), html.end(), header_regex);
  const std::sregex_iterator end;

  for (; current != end; ++current) {
    const std::smatch& match = *current;

    const std::size_t match_position =
        static_cast<std::size_t>(match.position(0));

    const std::size_t match_length =
        static_cast<std::size_t>(match.length(0));

    modified_html.append(
        html,
        last_position,
        match_position - last_position);

    std::string replacement = match.str(0);

    const std::size_t number_position =
    static_cast<std::size_t>(
        match.position(1) - match.position(0));

    replacement.replace(
        number_position,
        static_cast<std::size_t>(match.length(1)),
        IncrementNumber(match.str(1)));

    modified_html += replacement;

    last_position = match_position + match_length;
  }

  modified_html.append(html, last_position, std::string::npos);

  return modified_html;
}

/**
 MODIF
 * @brief Obtiene el nombre del fichero que contendrá la copia modificada.
 */
std::string GetModifiedFilename(const std::string& input_filename) {
  const std::size_t extension_position =
      input_filename.find_last_of('.');

  const std::size_t separator_position =
      input_filename.find_last_of("/\\");

  if (extension_position == std::string::npos ||
      (separator_position != std::string::npos &&
       extension_position < separator_position)) {
    return input_filename + "_modificada.html";
  }

  return input_filename.substr(0, extension_position) +
         "_modificada" +
         input_filename.substr(extension_position);
}

}

/**
 * @brief Escribe el esquema del documento HTML en un fichero.
 * @param output_filename Nombre del fichero de salida.
 * @param document Documento HTML ya analizado.
 * @return true si el fichero pudo generarse, false en caso contrario.
 */
bool ReportGenerator::Generate(const std::string& output_filename,
                               const HtmlDocument& document,
                               const std::string& input_filename) const { // modif


  // MODIF
  bool read_success = false;
  const std::string original_html =
      ReadTextFile(input_filename, &read_success);
  if (!read_success) {
    return false;
  }

  const std::string modified_html =
      IncrementHeaders(original_html);
  const std::string modified_filename =
      GetModifiedFilename(input_filename);

  std::ofstream modified_file(modified_filename);
  if (!modified_file.is_open()) {
    return false;
  }

  modified_file << modified_html;
  modified_file.close();


                                
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


  // MODIF: Añade al final la copia del HTML con las cabeceras modificadas.
  output_file << "\n\n";
  output_file << "HTML modificado :\n\n";
  output_file << modified_html;

  if (!modified_html.empty() &&
      modified_html.back() != '\n') {
    output_file << '\n';
  }

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