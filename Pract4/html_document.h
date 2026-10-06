/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Expresiones regulares en C++
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file html_document.h
 * @brief Definición del modelo de datos del documento HTML.
 * @date 06/10/2026
 * @version 1.0
 */

#ifndef P04_HTML_ANALYZER_HTML_DOCUMENT_H_
#define P04_HTML_ANALYZER_HTML_DOCUMENT_H_

#include <string>
#include <vector>

/**
 * @brief Almacena un atributo HTML con su nombre y valor.
 */
struct HtmlAttribute {
  std::string name;
  std::string value;
};

/**
 * @brief Representa una aparición de una etiqueta HTML permitida.
 */
struct HtmlTag {
  std::string name;
  int line;
  bool closing;
  bool has_attributes;
  int attribute_count;
};

/**
 * @brief Almacena los atributos asociados a una etiqueta de apertura.
 */
struct HtmlTagAttributes {
  std::string name;
  int line;
  std::vector<HtmlAttribute> attributes;
};

/**
 * @brief Representa un comentario HTML y las líneas que ocupa.
 */
struct HtmlComment {
  int start_line;
  int end_line;
  std::string text;
  bool description;
};

/**
 * @brief Representa y almacena la estructura general del HTML analizado.
 *
 * La clase separa los datos del documento de su análisis y de la generación
 * del fichero de salida.
 */
class HtmlDocument {
 public:
  /**
   * @brief Crea un documento HTML vacío asociado a un fichero de entrada.
   * @param program_name Nombre del fichero de entrada.
   */
  explicit HtmlDocument(const std::string& program_name);

  /**
   * @brief Obtiene el nombre del fichero de entrada.
   * @return Nombre del programa analizado.
   */
  const std::string& GetProgramName() const;

  /**
   * @brief Establece la descripción del documento.
   * @param description Texto del comentario que actúa como descripción.
   */
  void SetDescription(const std::string& description);

  /**
   * @brief Obtiene la descripción del documento.
   * @return Descripción almacenada, o una cadena vacía si no existe.
   */
  const std::string& GetDescription() const;

  /**
   * @brief Indica si se ha detectado la declaración DOCTYPE HTML5.
   * @return true si existe DOCTYPE HTML5, false en caso contrario.
   */
  bool HasDoctypeHtml5() const;

  /**
   * @brief Establece que el documento contiene DOCTYPE HTML5.
   */
  void SetDoctypeHtml5();

  /**
   * @brief Registra una etiqueta permitida encontrada en el documento.
   * @param tag Aparición de la etiqueta detectada.
   */
  void AddTag(const HtmlTag& tag);

  /**
   * @brief Registra los atributos encontrados en una etiqueta de apertura.
   * @param tag_attributes Información de la etiqueta y sus atributos.
   */
  void AddTagAttributes(const HtmlTagAttributes& tag_attributes);

  /**
   * @brief Registra un comentario del documento.
   * @param comment Comentario detectado.
   */
  void AddComment(const HtmlComment& comment);

  /**
   * @brief Comprueba si existe al menos una etiqueta con el nombre indicado.
   * @param tag_name Nombre de la etiqueta a consultar.
   * @return true si la etiqueta aparece, false en caso contrario.
   */
  bool HasTag(const std::string& tag_name) const;

  /**
   * @brief Obtiene todas las etiquetas detectadas en orden de aparición.
   * @return Referencia constante al conjunto de etiquetas.
   */
  const std::vector<HtmlTag>& GetTags() const;

  /**
   * @brief Obtiene las etiquetas de apertura que contienen atributos.
   * @return Referencia constante a las listas de atributos.
   */
  const std::vector<HtmlTagAttributes>& GetTagAttributes() const;

  /**
   * @brief Obtiene todos los comentarios detectados.
   * @return Referencia constante al conjunto de comentarios.
   */
  const std::vector<HtmlComment>& GetComments() const;

 private:
  // Datos del documento HTML
  std::string program_name_;
  std::string description_;
  bool doctype_html5_;
  std::vector<HtmlTag> tags_;
  std::vector<HtmlTagAttributes> tag_attributes_;
  std::vector<HtmlComment> comments_;
};

#endif