#include "HybridNitroXlsx.hpp"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>

namespace {

struct JsonParser {
  const char* p;
  explicit JsonParser(const char* s) : p(s) {}

  void skipWs() { while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') ++p; }

  char peek() { skipWs(); return *p; }

  void expect(char c) {
    skipWs();
    if (*p != c) throw std::runtime_error(std::string("Expected '") + c + "' but got '" + *p + "'");
    ++p;
  }

  std::string parseString() {
    expect('"');
    std::string s;
    while (*p && *p != '"') {
      if (*p == '\\') {
        ++p;
        switch (*p) {
          case '"':  s += '"';  break;
          case '\\': s += '\\'; break;
          case '/':  s += '/';  break;
          case 'b':  s += '\b'; break;
          case 'f':  s += '\f'; break;
          case 'n':  s += '\n'; break;
          case 'r':  s += '\r'; break;
          case 't':  s += '\t'; break;
          case 'u': {
            ++p;
            char hex[5] = {};
            std::memcpy(hex, p, 4);
            p += 4;
            unsigned long cp = std::strtoul(hex, nullptr, 16);
            if (cp < 0x80) {
              s += static_cast<char>(cp);
            } else if (cp < 0x800) {
              s += static_cast<char>(0xC0 | (cp >> 6));
              s += static_cast<char>(0x80 | (cp & 0x3F));
            } else {
              s += static_cast<char>(0xE0 | (cp >> 12));
              s += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
              s += static_cast<char>(0x80 | (cp & 0x3F));
            }
            continue;
          }
          default: s += '\\'; s += *p; break;
        }
      } else {
        s += *p;
      }
      ++p;
    }
    if (*p == '"') ++p;
    return s;
  }

  enum ValueType { T_STRING, T_NUMBER, T_BOOL, T_NULL };

  struct Value {
    ValueType type;
    std::string str;
    double num = 0;
    bool boolean = false;
  };

  Value parseValue() {
    skipWs();
    if (*p == '"') return { T_STRING, parseString(), 0, false };
    if (*p == 't') { p += 4; return { T_BOOL, "", 0, true }; }
    if (*p == 'f') { p += 5; return { T_BOOL, "", 0, false }; }
    if (*p == 'n') { p += 4; return { T_NULL, "", 0, false }; }

    // number
    Value v{ T_NUMBER, "", 0, false };
    const char* start = p;
    if (*p == '-') ++p;
    while (*p >= '0' && *p <= '9') ++p;
    if (*p == '.') { ++p; while (*p >= '0' && *p <= '9') ++p; }
    if (*p == 'e' || *p == 'E') { ++p; if (*p == '+' || *p == '-') ++p; while (*p >= '0' && *p <= '9') ++p; }
    v.num = std::strtod(start, nullptr);
    return v;
  }
};

} // anonymous namespace

namespace margelo::nitro::xlsx {

HybridNitroXlsx::HybridNitroXlsx()
    : HybridObject("NitroXlsx"), HybridNitroXlsxSpec() {
}

HybridNitroXlsx::~HybridNitroXlsx() {
}

std::shared_ptr<HybridXlsxWorkbookSpec> HybridNitroXlsx::createWorkbook(const std::optional<std::string>& tempDir) {
  return std::make_shared<HybridXlsxWorkbook>(tempDir);
}

std::shared_ptr<HybridXlsxWorkbookSpec> HybridNitroXlsx::openWorkbook(const std::string& path) {
  auto workbook = std::make_shared<HybridXlsxWorkbook>();
  workbook->openFromFile(path);
  return workbook;
}

std::shared_ptr<HybridXlsxWorkbookSpec> HybridNitroXlsx::openWorkbookFromBuffer(const std::shared_ptr<ArrayBuffer>& buffer) {
  auto workbook = std::make_shared<HybridXlsxWorkbook>();
  workbook->openFromBuffer(buffer->data(), buffer->size());
  return workbook;
}

std::shared_ptr<HybridXlsxWorkbookSpec> HybridNitroXlsx::fromJSON(const std::string& data) {
  auto workbook = std::make_shared<HybridXlsxWorkbook>();
  auto sheet = workbook->addWorksheet("Sheet1");

  JsonParser parser(data.c_str());
  parser.skipWs();
  parser.expect('[');

  if (parser.peek() == ']') return workbook;

  // Parse first object to extract keys
  parser.expect('{');

  std::vector<std::string> keys;
  unsigned int col = 1;

  while (parser.peek() != '}') {
    std::string key = parser.parseString();
    keys.push_back(key);
    parser.expect(':');
    auto val = parser.parseValue();

    switch (val.type) {
      case JsonParser::T_STRING:
        sheet->writeString(1, col, val.str, std::nullopt);
        break;
      case JsonParser::T_NUMBER:
        sheet->writeNumber(1, col, val.num, std::nullopt);
        break;
      case JsonParser::T_BOOL:
        sheet->writeBoolean(1, col, val.boolean, std::nullopt);
        break;
      default:
        break;
    }
    ++col;
    if (parser.peek() == ',') ++parser.p;
  }
  parser.expect('}');

  unsigned int row = 2;

  while (parser.peek() == ',') {
    ++parser.p;
    parser.expect('{');

    while (parser.peek() != '}') {
      std::string key = parser.parseString();
      parser.expect(':');
      auto val = parser.parseValue();

      unsigned int targetCol = 0;
      for (size_t k = 0; k < keys.size(); ++k) {
        if (keys[k] == key) { targetCol = static_cast<unsigned int>(k + 1); break; }
      }
      if (targetCol == 0) {
        keys.push_back(key);
        targetCol = static_cast<unsigned int>(keys.size());
      }

      switch (val.type) {
        case JsonParser::T_STRING:
          sheet->writeString(row, targetCol, val.str, std::nullopt);
          break;
        case JsonParser::T_NUMBER:
          sheet->writeNumber(row, targetCol, val.num, std::nullopt);
          break;
        case JsonParser::T_BOOL:
          sheet->writeBoolean(row, targetCol, val.boolean, std::nullopt);
          break;
        default:
          break;
      }

      if (parser.peek() == ',') ++parser.p;
    }
    parser.expect('}');
    ++row;
  }

  parser.expect(']');
  return workbook;
}

}