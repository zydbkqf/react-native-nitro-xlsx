#include "HybridXlsxCellFormat.hpp"

namespace margelo::nitro::xlsx {

HybridXlsxCellFormat::HybridXlsxCellFormat()
    : HybridObject("XlsxCellFormat"), HybridXlsxCellFormatSpec() {
}

HybridXlsxCellFormat::~HybridXlsxCellFormat() {
}

OpenXLSX::XLStyleIndex HybridXlsxCellFormat::applyToDocument(OpenXLSX::XLStyles& styles, uint64_t documentGeneration) {
  return _fmt.applyToDocument(styles, documentGeneration);
}

void HybridXlsxCellFormat::applyToCell(OpenXLSX::XLCell& cell, OpenXLSX::XLStyleIndex styleIndex) {
  _fmt.applyToCell(cell, styleIndex);
}

void HybridXlsxCellFormat::setFontName(const std::string& name) {
  _fmt.setFontName(name);
}

void HybridXlsxCellFormat::setFontSize(double size) {
  _fmt.setFontSize(size);
}

void HybridXlsxCellFormat::setFontColor(double color) {
  _fmt.setFontColor(color);
}

void HybridXlsxCellFormat::setFontScript(double style) {
  _fmt.setFontScript(style);
}

void HybridXlsxCellFormat::setBold() {
  _fmt.setBold();
}

void HybridXlsxCellFormat::setItalic() {
  _fmt.setItalic();
}

void HybridXlsxCellFormat::setUnderline(std::optional<double> style) {
  _fmt.setUnderline(style);
}

void HybridXlsxCellFormat::setStrikeout() {
  _fmt.setStrikeout();
}

void HybridXlsxCellFormat::setAlign(double align) {
  _fmt.setAlign(align);
}

void HybridXlsxCellFormat::setVerticalAlign(double align) {
  _fmt.setVerticalAlign(align);
}

void HybridXlsxCellFormat::setTextWrap() {
  _fmt.setTextWrap();
}

void HybridXlsxCellFormat::setRotation(double angle) {
  _fmt.setRotation(angle);
}

void HybridXlsxCellFormat::setIndent(double level) {
  _fmt.setIndent(level);
}

void HybridXlsxCellFormat::setShrink() {
  _fmt.setShrink();
}

void HybridXlsxCellFormat::setNumFormat(const std::string& format) {
  _fmt.setNumFormat(format);
}

void HybridXlsxCellFormat::setNumFormatIndex(double index) {
  _fmt.setNumFormatIndex(index);
}

void HybridXlsxCellFormat::setBgColor(double color) {
  _fmt.setBgColor(color);
}

void HybridXlsxCellFormat::setFgColor(double color) {
  _fmt.setFgColor(color);
}

void HybridXlsxCellFormat::setPattern(double pattern) {
  _fmt.setPattern(pattern);
}

void HybridXlsxCellFormat::setBorder(double style) {
  _fmt.setBorder(style);
}

void HybridXlsxCellFormat::setBorderColor(double color) {
  _fmt.setBorderColor(color);
}

void HybridXlsxCellFormat::setBottom(double style) {
  _fmt.setBottom(style);
}

void HybridXlsxCellFormat::setTop(double style) {
  _fmt.setTop(style);
}

void HybridXlsxCellFormat::setLeft(double style) {
  _fmt.setLeft(style);
}

void HybridXlsxCellFormat::setRight(double style) {
  _fmt.setRight(style);
}

void HybridXlsxCellFormat::setBottomColor(double color) {
  _fmt.setBottomColor(color);
}

void HybridXlsxCellFormat::setTopColor(double color) {
  _fmt.setTopColor(color);
}

void HybridXlsxCellFormat::setLeftColor(double color) {
  _fmt.setLeftColor(color);
}

void HybridXlsxCellFormat::setRightColor(double color) {
  _fmt.setRightColor(color);
}

void HybridXlsxCellFormat::setDiagonal(double style) {
  _fmt.setDiagonal(style);
}

void HybridXlsxCellFormat::setDiagonalColor(double color) {
  _fmt.setDiagonalColor(color);
}

void HybridXlsxCellFormat::setLocked() {
  _fmt.setLocked();
}

void HybridXlsxCellFormat::setUnlocked() {
  _fmt.setUnlocked();
}

void HybridXlsxCellFormat::setHyperlink() {
  _fmt.setHyperlink();
}

void HybridXlsxCellFormat::setFontOnly() {
  _fmt.setFontOnly();
}

std::string HybridXlsxCellFormat::getFontName() {
  return _fmt.getFontName();
}

double HybridXlsxCellFormat::getFontSize() {
  return _fmt.getFontSize();
}

double HybridXlsxCellFormat::getFontColor() {
  return _fmt.getFontColor();
}

bool HybridXlsxCellFormat::getIsBold() {
  return _fmt.getIsBold();
}

bool HybridXlsxCellFormat::getIsItalic() {
  return _fmt.getIsItalic();
}

std::string HybridXlsxCellFormat::getNumFormat() {
  return _fmt.getNumFormat();
}

double HybridXlsxCellFormat::getBgColor() {
  return _fmt.getBgColor();
}

}
