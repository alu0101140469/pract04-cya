/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Expresiones regulares en C++
 * @author Daniel Palenzuela Álvarez
 * Correo: alu0101140469@ull.edu.es
 * @file html_document.cc
 * @brief Implementación del modelo de datos del documento HTML.
 * @date 06/10/2026
 * @version 1.0
 */

#include "html_document.h"

#include <algorithm>

/**
 * @brief Crea un documento HTML vacío asociado a un fichero de entrada.
 * @param program_name Nombre del fichero de entrada.
 */
HtmlDocument::HtmlDocument(const std::string& program_name)
    : program_name_(program_name), doctype_html5_(false) {}

/**
 * @brief Obtiene el nombre del fichero de entrada.
 * @return Nombre del programa analizado.
 */
const std::string& HtmlDocument::GetProgramName() const {
  return program_name_;
}

/**
 * @brief Establece la descripción del documento.
 * @param description Texto del comentario que actúa como descripción.
 */
void HtmlDocument::SetDescription(const std::string& description) {
  description_ = description;
}

/**
 * @brief Obtiene la descripción del documento.
 * @return Descripción almacenada, o una cadena vacía si no existe.
 */
const std::string& HtmlDocument::GetDescription() const {
  return description_;
}

/**
 * @brief Indica si se ha detectado la declaración DOCTYPE HTML5.
 * @return true si existe DOCTYPE HTML5, false en caso contrario.
 */
bool HtmlDocument::HasDoctypeHtml5() const {
  return doctype_html5_;
}

/**
 * @brief Establece que el documento contiene DOCTYPE HTML5.
 */
void HtmlDocument::SetDoctypeHtml5() {
  doctype_html5_ = true;
}

/**
 * @brief Añade una etiqueta al documento.
 * @param tag Etiqueta a añadir.
 */
void HtmlDocument::AddTag(const HtmlTag& tag) {
  tags_.push_back(tag);
}

/**
 * @brief Añade los atributos de una etiqueta al documento.
 * @param tag_attributes Atributos de la etiqueta a añadir.
 */
void HtmlDocument::AddTagAttributes(const HtmlTagAttributes& tag_attributes) {
  tag_attributes_.push_back(tag_attributes);
}

/**
 * @brief Añade un comentario al documento.
 * @param comment Comentario a añadir.
 */
void HtmlDocument::AddComment(const HtmlComment& comment) {
  comments_.push_back(comment);
}

/**
 * @brief Comprueba si existe al menos una etiqueta con el nombre indicado.
 * @param tag_name Nombre de la etiqueta a consultar.
 * @return true si la etiqueta aparece, false en caso contrario.
 */
bool HtmlDocument::HasTag(const std::string& tag_name) const {
  return std::any_of(
      tags_.begin(), tags_.end(),
      [&tag_name](const HtmlTag& tag) { return tag.name == tag_name; });
}

/**
 * @brief Obtiene la lista de etiquetas del documento.
 * @return Vector con las etiquetas encontradas.
 */
const std::vector<HtmlTag>& HtmlDocument::GetTags() const {
  return tags_;
}

/**
 * @brief Obtiene la lista de atributos de las etiquetas del documento.
 * @return Vector con los atributos encontrados.
 */
const std::vector<HtmlTagAttributes>& HtmlDocument::GetTagAttributes() const {
  return tag_attributes_;
}

/**
 * @brief Obtiene la lista de comentarios del documento.
 * @return Vector con los comentarios encontrados.
 */
const std::vector<HtmlComment>& HtmlDocument::GetComments() const {
  return comments_;
}