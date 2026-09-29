#pragma once

#include <OpenXLSX/XLException.hpp>
#include <stdexcept>
#include <string>

namespace margelo::nitro::xlsx {

/**
 * Structured error with a stable code that the JS side can switch on.
 * Message format written to JS: "CODE: message" (Nitro prefixes the method name).
 */
class XlsxError : public std::runtime_error {
public:
  XlsxError(std::string code, const std::string& message)
      : std::runtime_error(code + ": " + message), _code(std::move(code)) {}

  const std::string& code() const noexcept { return _code; }

private:
  std::string _code;
};

namespace xlsx_error {
inline constexpr const char* INVALID_ARGUMENT   = "INVALID_ARGUMENT";
inline constexpr const char* SHEET_NOT_FOUND    = "SHEET_NOT_FOUND";
inline constexpr const char* SHEET_EXISTS       = "SHEET_EXISTS";
inline constexpr const char* INDEX_OUT_OF_RANGE = "INDEX_OUT_OF_RANGE";
inline constexpr const char* CELL_NOT_FOUND     = "CELL_NOT_FOUND";
inline constexpr const char* WORKBOOK_CLOSED    = "WORKBOOK_CLOSED";
inline constexpr const char* IO_ERROR           = "IO_ERROR";
inline constexpr const char* UNSUPPORTED        = "UNSUPPORTED";
inline constexpr const char* XLSX_ERROR         = "XLSX_ERROR";
inline constexpr const char* INTERNAL_ERROR     = "INTERNAL_ERROR";
} // namespace xlsx_error

/**
 * Map OpenXLSX / std exceptions to XlsxError so the JS side always sees "CODE: message".
 * Anything already an XlsxError is rethrown unchanged.
 */
template <typename Fn>
auto withXlsxError(const char* defaultCode, Fn&& fn) -> decltype(fn()) {
  try {
    return fn();
  } catch (const XlsxError&) {
    throw;
  } catch (const OpenXLSX::XLInputError& e) {
    throw XlsxError(xlsx_error::INVALID_ARGUMENT, e.what());
  } catch (const OpenXLSX::XLCellAddressError& e) {
    throw XlsxError(xlsx_error::INVALID_ARGUMENT, e.what());
  } catch (const OpenXLSX::XLSheetError& e) {
    throw XlsxError(xlsx_error::SHEET_NOT_FOUND, e.what());
  } catch (const OpenXLSX::XLOverflowError& e) {
    throw XlsxError(xlsx_error::INDEX_OUT_OF_RANGE, e.what());
  } catch (const OpenXLSX::XLValueTypeError& e) {
    throw XlsxError(xlsx_error::INVALID_ARGUMENT, e.what());
  } catch (const OpenXLSX::XLFormulaError& e) {
    throw XlsxError(xlsx_error::INVALID_ARGUMENT, e.what());
  } catch (const OpenXLSX::XLException& e) {
    throw XlsxError(defaultCode, e.what());
  } catch (const std::exception& e) {
    throw XlsxError(defaultCode, e.what());
  } catch (...) {
    throw XlsxError(xlsx_error::INTERNAL_ERROR, "Unknown native error");
  }
}

} // namespace margelo::nitro::xlsx
