#pragma once

#include "HybridXlsxConditionalFormatSpec.hpp"
#include <OpenXLSX/XLSheet.hpp>
#include <memory>
#include <string>

namespace margelo::nitro::xlsx {

class HybridXlsxConditionalFormat : public HybridXlsxConditionalFormatSpec {
public:
  explicit HybridXlsxConditionalFormat(OpenXLSX::XLConditionalFormat format);
  ~HybridXlsxConditionalFormat() override;

  std::string getSqref() override;
  void setSqref(const std::string& sqref) override;

  double getRuleCount() override;
  double createRule() override;
  bool deleteRule(double index) override;

  double getRuleType(double index) override;
  void setRuleType(double index, double type) override;
  double getRuleDxfId(double index) override;
  void setRuleDxfId(double index, double dxfId) override;

  std::string getRuleFormula(double index, double formulaIndex) override;
  void setRuleFormula(double index, double formulaIndex, const std::string& formula) override;

  double getRuleOperator(double index) override;
  void setRuleOperator(double index, double op) override;
  std::string getRuleText(double index) override;
  void setRuleText(double index, const std::string& text) override;
  double getRulePriority(double index) override;
  void setRulePriority(double index, double priority) override;
  bool getRuleStopIfTrue(double index) override;
  void setRuleStopIfTrue(double index, bool stop) override;
  double getRuleTimePeriod(double index) override;
  void setRuleTimePeriod(double index, double period) override;
  double getRuleRank(double index) override;
  void setRuleRank(double index, double rank) override;
  double getRuleStdDev(double index) override;
  void setRuleStdDev(double index, double stdDev) override;
  bool getRuleAboveAverage(double index) override;
  void setRuleAboveAverage(double index, bool set) override;
  bool getRulePercent(double index) override;
  void setRulePercent(double index, bool set) override;
  bool getRuleBottom(double index) override;
  void setRuleBottom(double index, bool set) override;
  bool getRuleEqualAverage(double index) override;
  void setRuleEqualAverage(double index, bool set) override;

  std::string summary() override;

private:
  OpenXLSX::XLConditionalFormat _format;

  OpenXLSX::XLCfRule ruleAt(double index);
  OpenXLSX::XLCfRules rules();
};

} // namespace margelo::nitro::xlsx
