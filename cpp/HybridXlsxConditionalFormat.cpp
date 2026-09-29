#include "HybridXlsxConditionalFormat.hpp"
#include "XlsxError.hpp"

namespace margelo::nitro::xlsx {

HybridXlsxConditionalFormat::HybridXlsxConditionalFormat(OpenXLSX::XLConditionalFormat format)
    : HybridObject("XlsxConditionalFormat"), HybridXlsxConditionalFormatSpec(), _format(std::move(format)) {
}

HybridXlsxConditionalFormat::~HybridXlsxConditionalFormat() = default;

OpenXLSX::XLCfRules HybridXlsxConditionalFormat::rules() {
  return _format.cfRules();
}

OpenXLSX::XLCfRule HybridXlsxConditionalFormat::ruleAt(double index) {
  size_t idx = static_cast<size_t>(index);
  auto cfRules = _format.cfRules();
  if (idx >= cfRules.count()) {
    throw XlsxError(xlsx_error::INDEX_OUT_OF_RANGE,
                    "cfRule index " + std::to_string(idx) + " out of range (count=" + std::to_string(cfRules.count()) + ")");
  }
  return cfRules.cfRuleByIndex(idx);
}

std::string HybridXlsxConditionalFormat::getSqref() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _format.sqref(); });
}

void HybridXlsxConditionalFormat::setSqref(const std::string& sqref) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!_format.setSqref(sqref)) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set sqref: " + sqref);
    }
  });
}

double HybridXlsxConditionalFormat::getRuleCount() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(_format.cfRules().count());
  });
}

double HybridXlsxConditionalFormat::createRule() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    auto cfRules = _format.cfRules();
    return static_cast<double>(cfRules.create());
  });
}

bool HybridXlsxConditionalFormat::deleteRule(double index) {
  // OpenXLSX does not expose cfRule deletion
  (void)index;
  return false;
}

double HybridXlsxConditionalFormat::getRuleType(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(ruleAt(index).type());
  });
}

void HybridXlsxConditionalFormat::setRuleType(double index, double type) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setType(static_cast<OpenXLSX::XLCfType>(static_cast<uint8_t>(type)))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set rule type");
    }
  });
}

double HybridXlsxConditionalFormat::getRuleDxfId(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(ruleAt(index).dxfId());
  });
}

void HybridXlsxConditionalFormat::setRuleDxfId(double index, double dxfId) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setDxfId(static_cast<OpenXLSX::XLStyleIndex>(dxfId))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set rule dxfId");
    }
  });
}

std::string HybridXlsxConditionalFormat::getRuleFormula(double index, double formulaIndex) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    // OpenXLSX only supports the primary <formula> node
    if (formulaIndex != 0) return std::string();
    return ruleAt(index).formula();
  });
}

void HybridXlsxConditionalFormat::setRuleFormula(double index, double formulaIndex, const std::string& formula) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (formulaIndex != 0) {
      throw XlsxError(xlsx_error::UNSUPPORTED,
                      "OpenXLSX only supports a single formula per cfRule (formulaIndex must be 0)");
    }
    if (!ruleAt(index).setFormula(formula)) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set rule formula");
    }
  });
}

double HybridXlsxConditionalFormat::getRuleOperator(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(ruleAt(index).Operator());
  });
}

void HybridXlsxConditionalFormat::setRuleOperator(double index, double op) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setOperator(static_cast<OpenXLSX::XLCfOperator>(static_cast<uint8_t>(op)))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set rule operator");
    }
  });
}

std::string HybridXlsxConditionalFormat::getRuleText(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return ruleAt(index).text(); });
}

void HybridXlsxConditionalFormat::setRuleText(double index, const std::string& text) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setText(text)) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set rule text");
    }
  });
}

double HybridXlsxConditionalFormat::getRulePriority(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(ruleAt(index).priority());
  });
}

void HybridXlsxConditionalFormat::setRulePriority(double index, double priority) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    auto cfRules = _format.cfRules();
    if (!cfRules.setPriority(static_cast<size_t>(index), static_cast<uint16_t>(priority))) {
      throw XlsxError(xlsx_error::INDEX_OUT_OF_RANGE, "Failed to set rule priority");
    }
  });
}

bool HybridXlsxConditionalFormat::getRuleStopIfTrue(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return ruleAt(index).stopIfTrue(); });
}

void HybridXlsxConditionalFormat::setRuleStopIfTrue(double index, bool stop) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setStopIfTrue(stop)) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set stopIfTrue");
    }
  });
}

double HybridXlsxConditionalFormat::getRuleTimePeriod(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(ruleAt(index).timePeriod());
  });
}

void HybridXlsxConditionalFormat::setRuleTimePeriod(double index, double period) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setTimePeriod(static_cast<OpenXLSX::XLCfTimePeriod>(static_cast<uint8_t>(period)))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set timePeriod");
    }
  });
}

double HybridXlsxConditionalFormat::getRuleRank(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(ruleAt(index).rank());
  });
}

void HybridXlsxConditionalFormat::setRuleRank(double index, double rank) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setRank(static_cast<uint16_t>(rank))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set rank");
    }
  });
}

double HybridXlsxConditionalFormat::getRuleStdDev(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    return static_cast<double>(ruleAt(index).stdDev());
  });
}

void HybridXlsxConditionalFormat::setRuleStdDev(double index, double stdDev) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setStdDev(static_cast<int16_t>(stdDev))) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set stdDev");
    }
  });
}

bool HybridXlsxConditionalFormat::getRuleAboveAverage(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return ruleAt(index).aboveAverage(); });
}

void HybridXlsxConditionalFormat::setRuleAboveAverage(double index, bool set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setAboveAverage(set)) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set aboveAverage");
    }
  });
}

bool HybridXlsxConditionalFormat::getRulePercent(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return ruleAt(index).percent(); });
}

void HybridXlsxConditionalFormat::setRulePercent(double index, bool set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setPercent(set)) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set percent");
    }
  });
}

bool HybridXlsxConditionalFormat::getRuleBottom(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return ruleAt(index).bottom(); });
}

void HybridXlsxConditionalFormat::setRuleBottom(double index, bool set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setBottom(set)) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set bottom");
    }
  });
}

bool HybridXlsxConditionalFormat::getRuleEqualAverage(double index) {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return ruleAt(index).equalAverage(); });
}

void HybridXlsxConditionalFormat::setRuleEqualAverage(double index, bool set) {
  withXlsxError(xlsx_error::XLSX_ERROR, [&] {
    if (!ruleAt(index).setEqualAverage(set)) {
      throw XlsxError(xlsx_error::XLSX_ERROR, "Failed to set equalAverage");
    }
  });
}

std::string HybridXlsxConditionalFormat::summary() {
  return withXlsxError(xlsx_error::XLSX_ERROR, [&] { return _format.summary(); });
}

} // namespace margelo::nitro::xlsx
