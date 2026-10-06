/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Expresiones regulares en C++
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file html_analyzer.cc
 * @brief Implementación del analizador HTML mediante expresiones regulares.
 * @date 06/10/2026
 * @version 1.0
 */

#include "html_analyzer.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>

/**
 * @brief Construye un analizador con los patrones de la práctica.
 */
HtmlAnalyzer::HtmlAnalyzer()
    : tag_regex_(R"(<\s*(/?)\s*(html|head|title|body|h1|p|a|img)\b([^>]*)>)", // Etiquetas HTML
                 std::regex::icase),
      attribute_regex_(
          R"re(([A-Za-z_:][A-Za-z0-9:._-]*)\s*=\s*"([^"]*)")re"), // Atributos de las etiquetas
      doctype_regex_(R"(<!DOCTYPE\s+html\s*>)", std::regex::icase), // DOCTYPE HTML5
      comment_regex_(R"(<!--([\s\S]*?)-->)"), // Comentarios HTML
      allowed_tag_regex_(R"((html|head|title|body|h1|p|a|img))", // Etiquetas permitidas
                         std::regex::icase),
      only_whitespace_regex_(R"(\s*)") {} // Solo espacios en blanco

/**
 * @brief Analiza el fichero indicado y rellena un documento HTML.
 * @param input_filename Nombre del fichero HTML de entrada.
 * @param document Documento donde se almacenará el resultado.
 * @return true si el fichero pudo abrirse y analizarse, false si falló.
 */
bool HtmlAnalyzer::Analyze(const std::string& input_filename,
                           HtmlDocument* document) const {
  if (document == nullptr) {
    return false;
  }

  /**
   * @brief Lee el contenido de un fichero y devuelve su contenido como cadena.
   * @param input_filename Nombre del fichero de entrada.
   * @param success Puntero a un booleano que se establecerá a true si el fichero se lee correctamente, false en caso contrario.
   * @return El contenido del fichero como cadena, o una cadena vacía si no se puede leer.
   */
  bool success = false;
  const std::string source = ReadFile(input_filename, &success);
  if (!success) {
    return false;
  }

  const std::size_t source_size = source.size(); // Tamaño del contenido del fichero
  std::size_t doctype_position = source_size; // Posición del DOCTYPE en el contenido
  std::size_t doctype_length = 0; // Longitud del DOCTYPE en el contenido

  /**
   * @brief Analiza la declaración DOCTYPE y la almacena en el documento.
   * @param source El contenido del fichero de entrada.
   * @param document El documento donde se almacenará el resultado.
   * @param position Puntero a la posición del DOCTYPE en el contenido.
   * @param length Puntero a la longitud del DOCTYPE en el contenido.
   */
  AnalyzeDoctype(source, document, &doctype_position, &doctype_length);

  /**
   * @brief Analiza las etiquetas HTML y las almacena en el documento.
   * @param source El contenido del fichero de entrada.
   * @param document El documento donde se almacenará el resultado.
   */
  AnalyzeTags(source, document);

  /**
   * @brief Analiza los comentarios HTML y los almacena en el documento.
   * @param source El contenido del fichero de entrada.
   * @param document El documento donde se almacenará el resultado.
   * @param doctype_position La posición del DOCTYPE en el contenido.
   * @param doctype_length La longitud del DOCTYPE en el contenido.
   */
  AnalyzeComments(source, document, doctype_position, doctype_length);

  return true;
}

/**
 * @brief Lee el contenido de un fichero y devuelve su contenido como cadena.
 * @param input_filename Nombre del fichero de entrada.
 * @param success Puntero a un booleano que se establecerá a true si el fichero se lee correctamente, false en caso contrario.
 * @return El contenido del fichero como cadena, o una cadena vacía si no se puede leer.
 */
std::string HtmlAnalyzer::ReadFile(const std::string& input_filename,
                                   bool* success) {
  // Abre el fichero de entrada                                  
  std::ifstream input_file(input_filename);

  // Comprueba si el fichero se abrió correctamente
  if (!input_file.is_open()) {
    *success = false;
    return std::string();
  }

  // Lee el contenido del fichero en una cadena
  std::string source((std::istreambuf_iterator<char>(input_file)),
                     std::istreambuf_iterator<char>());

  *success = true;
  return source;
}

/**
 * @brief Obtiene el número de línea correspondiente a una posición en el texto.
 * @param text El contenido del fichero de entrada.
 * @param position La posición en el texto para la que se desea obtener el número de línea.
 * @return El número de línea correspondiente a la posición indicada.
 */
int HtmlAnalyzer::GetLineNumber(const std::string& text,
                                std::size_t position) {
  int line_number = 1;

  // Recorre el texto hasta la posición indicada y cuenta las líneas
  for (std::size_t index = 0;
       index < position && index < text.size(); ++index) {
    if (text[index] == '\n') {
      ++line_number;
    }
  }

  return line_number;
}

/**
 * @brief Comprueba si un comentario es el que actúa como descripción del documento.
 * @param source El contenido del fichero de entrada.
 * @param doctype_end La posición final del DOCTYPE en el contenido.
 * @param comment_start La posición inicial del comentario en el contenido.
 * @return true si el comentario es la descripción, false en caso contrario.
 */
bool HtmlAnalyzer::IsDescriptionComment(const std::string& source,
                                         std::size_t doctype_end,
                                         std::size_t comment_start) const {

  // Comprueba si el comentario está después del DOCTYPE y antes del final del contenido                                  
  if (doctype_end > comment_start || comment_start > source.size()) {
    return false;
  }

  // Comprueba si el texto entre el DOCTYPE y el comentario contiene solo espacios en blanco
  return std::regex_match(
      source.substr(doctype_end, comment_start - doctype_end),
      only_whitespace_regex_);
}

/**
 * @brief Analiza las etiquetas HTML y las almacena en el documento.
 * @param source El contenido del fichero de entrada.
 * @param document El documento donde se almacenará el resultado.
 */
void HtmlAnalyzer::AnalyzeTags(const std::string& source,
                               HtmlDocument* document) const {
  // Crea una versión del contenido con los comentarios enmascarados para evitar que interfieran con el análisis de etiquetas                              
  const std::string search_source = MaskComments(source);

  // Itera sobre todas las coincidencias de etiquetas HTML en el contenido
  std::sregex_iterator current(search_source.begin(), search_source.end(),
                               tag_regex_);
  const std::sregex_iterator end;

  // Recorre todas las coincidencias de etiquetas HTML encontradas
  for (; current != end; ++current) {
    const std::smatch& match = *current;
    std::string tag_name = match[2].str();

    // Comprueba si la etiqueta es una de las permitidas
    if (!std::regex_match(tag_name, allowed_tag_regex_)) {
      continue;
    }

    // Determina si la etiqueta es de cierre, para ello se comprueba si el primer grupo de captura (/) está presente
    const bool closing = !match[1].str().empty();

    // Convierte el nombre de la etiqueta a minúsculas para un análisis uniforme
    std::transform(tag_name.begin(), tag_name.end(), tag_name.begin(),
                   [](unsigned char character) {
                     return static_cast<char>(std::tolower(character));
                   });

    // Ignora las etiquetas de cierre de <img> ya que no tienen contenido ni atributos               
    if (closing && tag_name == "img") {
      continue;
    }

    // Obtiene la posición de la etiqueta en el contenido y calcula el número de línea correspondiente
    const std::size_t tag_position =
        static_cast<std::size_t>(match.position(0));

    // Obtiene el número de línea correspondiente a la posición de la etiqueta
    const int line = GetLineNumber(source, tag_position);
    const std::string attributes_text = match[3].str();

    // Crea un objeto HtmlTagAttributes para almacenar los atributos de la etiqueta
    HtmlTagAttributes tag_attributes;
    tag_attributes.name = tag_name;
    tag_attributes.line = line;

    // Si la etiqueta no es de cierre, analiza sus atributos y los almacena en el objeto HtmlTagAttributes
    if (!closing) {
      std::sregex_iterator attribute_current(
          attributes_text.begin(), attributes_text.end(), attribute_regex_);

      // Itera sobre todas las coincidencias de atributos en el texto de atributos
      for (; attribute_current != end; ++attribute_current) {
        const std::smatch& attribute_match = *attribute_current;

        // Añade el atributo al objeto HtmlTagAttributes
        tag_attributes.attributes.push_back(HtmlAttribute{
            attribute_match[1].str(), attribute_match[2].str()});
      }
    }

    // Determina si la etiqueta tiene atributos y cuenta el número de atributos encontrados
    const bool has_attributes = !tag_attributes.attributes.empty();

    // Crea un objeto HtmlTag con la información de la etiqueta y lo añade al documento
    const int attribute_count =
        static_cast<int>(tag_attributes.attributes.size());

    // Añade la etiqueta al documento, incluyendo su información de línea, si es de cierre, si tiene atributos y el número de atributos
    document->AddTag(HtmlTag{tag_name, line, closing, has_attributes,
                             attribute_count});

    // Si la etiqueta tiene atributos, añade la información de los atributos al documento                         
    if (has_attributes) {
      document->AddTagAttributes(tag_attributes);
    }
  }
}

/**
 * @brief Enmascara los comentarios en el contenido del fichero para evitar que interfieran con el análisis de etiquetas.
 * @param source El contenido del fichero de entrada.
 * @return Una copia del contenido con los comentarios reemplazados por espacios en blanco.
 */
std::string HtmlAnalyzer::MaskComments(const std::string& source) const {
  std::string masked_source = source;

  // Itera sobre todas las coincidencias de comentarios HTML en el contenido
  std::sregex_iterator current(source.begin(), source.end(), comment_regex_);
  const std::sregex_iterator end;

  // Recorre todas las coincidencias de comentarios HTML encontradas
  for (; current != end; ++current) {
    const std::smatch& match = *current;

    // Obtiene la posición y longitud del comentario en el contenido
    const std::size_t position =
        static_cast<std::size_t>(match.position(0));

    // Reemplaza el comentario por espacios en blanco, manteniendo los saltos de línea para preservar la numeración de líneas
    const std::size_t length = match.length(0);

    // Recorre la longitud del comentario y reemplaza los caracteres por espacios en blanco, excepto los saltos de línea
    for (std::size_t index = position; index < position + length; ++index) {
      if (masked_source[index] != '\n' &&
          masked_source[index] != '\r') {
        masked_source[index] = ' ';
      }
    }
  }

  return masked_source;
}

/**
 * @brief Analiza los comentarios HTML y los almacena en el documento.
 * @param source El contenido del fichero de entrada.
 * @param document El documento donde se almacenará el resultado.
 * @param doctype_position La posición del DOCTYPE en el contenido.
 * @param doctype_length La longitud del DOCTYPE en el contenido.
 */
void HtmlAnalyzer::AnalyzeComments(const std::string& source,
                                    HtmlDocument* document,
                                    std::size_t doctype_position,
                                    std::size_t doctype_length) const {
  // Itera sobre todas las coincidencias de comentarios HTML en el contenido                              
  std::sregex_iterator current(source.begin(), source.end(), comment_regex_);
  const std::sregex_iterator end;

  // Variable para rastrear si es el primer comentario encontrado
  bool first_comment = true;

  // Recorre todas las coincidencias de comentarios HTML encontradas
  for (; current != end; ++current) {
    const std::smatch& match = *current;

    // Obtiene la posición inicial y final del comentario en el contenido
    const std::size_t comment_position =
        static_cast<std::size_t>(match.position(0));

    const std::size_t comment_end_position =
        comment_position + match.length(0) - 1;

    // Determina si el comentario es la descripción del documento, 
    // basándose en su posición respecto al DOCTYPE y si es el primer comentario encontrado
    const bool description =
        first_comment &&
        doctype_position < source.size() &&
        IsDescriptionComment(source,
                             doctype_position + doctype_length,
                             comment_position);

    // Crea un objeto HtmlComment con la información del comentario y lo añade al documento                    
    HtmlComment comment;

    // Obtiene el número de línea correspondiente a la posición inicial del comentario
    comment.start_line =
        GetLineNumber(source, comment_position);

    // Obtiene el número de línea correspondiente a la posición final del comentario
    comment.end_line =
        GetLineNumber(source, comment_end_position);

    // Almacena el texto del comentario y si es la descripción del documento
    comment.text = match.str(0);
    comment.description = description;

    // Si el comentario es la descripción, se limpia el texto eliminando espacios en blanco al inicio y al final
    if (description) {
      std::string description_text = match.str(1);

      // Elimina los espacios en blanco al inicio y al final del texto de la descripción
      const std::size_t first =
          description_text.find_first_not_of(" \t\r\n");

      const std::size_t last =
          description_text.find_last_not_of(" \t\r\n");

      // Si se encontraron caracteres distintos de espacios en blanco, se recorta el texto 
      // si no, se establece como cadena vacía
      if (first != std::string::npos) {
        description_text =
            description_text.substr(first, last - first + 1);
      } else {
        description_text.clear();
      }

      // Establece la descripción del documento en el objeto HtmlDocument
      document->SetDescription(description_text);
    }

    // Añade el comentario al documento
    document->AddComment(comment);
    // Marca que ya se ha procesado el primer comentario
    first_comment = false;
  }
}

/**
 * @brief Analiza la declaración DOCTYPE y la almacena en el documento.
 * @param source El contenido del fichero de entrada.
 * @param document El documento donde se almacenará el resultado.
 * @param position Puntero a la posición del DOCTYPE en el contenido.
 * @param length Puntero a la longitud del DOCTYPE en el contenido.
 */
void HtmlAnalyzer::AnalyzeDoctype(const std::string& source,
                                  HtmlDocument* document,
                                  std::size_t* position,
                                  std::size_t* length) const {
  std::smatch match;

  // Busca la coincidencia del DOCTYPE HTML5 en el contenido
  if (!std::regex_search(source, match, doctype_regex_)) {
    return;
  }

  // Si se encuentra el DOCTYPE, se establece en el documento que contiene DOCTYPE HTML5
  document->SetDoctypeHtml5();

  // Se actualizan la posición y longitud del DOCTYPE en el contenido
  *position = static_cast<std::size_t>(match.position(0));
  *length = match.length(0);
}