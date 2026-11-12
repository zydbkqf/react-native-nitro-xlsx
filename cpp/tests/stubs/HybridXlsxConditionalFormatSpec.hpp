#pragma once

// Test-build stand-in for nitrogen's HybridXlsxConditionalFormatSpec.hpp.
// Mirrors the pure-virtual interface (signatures must stay in sync) without the
// Nitro/JSI hybrid-registration machinery, so the real
// HybridXlsxConditionalFormat implementation compiles under gtest.

#include <NitroModules/HybridObject.hpp>
#include <string>

namespace margelo::nitro::xlsx {

  using namespace margelo::nitro;

  class HybridXlsxConditionalFormatSpec: public virtual HybridObject {
    public:
      explicit HybridXlsxConditionalFormatSpec(): HybridObject(TAG) { }
      ~HybridXlsxConditionalFormatSpec() override = default;

    public:
      virtual std::string getSqref() = 0;
      virtual void setSqref(const std::string& sqref) = 0;
      virtual double getRuleCount() = 0;
      virtual double createRule() = 0;
      virtual bool deleteRule(double index) = 0;
      virtual double getRuleType(double index) = 0;
      virtual void setRuleType(double index, double type) = 0;
      virtual double getRuleDxfId(double index) = 0;
      virtual void setRuleDxfId(double index, double dxfId) = 0;
      virtual std::string getRuleFormula(double index, double formulaIndex) = 0;
      virtual void setRuleFormula(double index, double formulaIndex, const std::string& formula) = 0;
      virtual double getRuleOperator(double index) = 0;
      virtual void setRuleOperator(double index, double op) = 0;
      virtual std::string getRuleText(double index) = 0;
      virtual void setRuleText(double index, const std::string& text) = 0;
      virtual double getRulePriority(double index) = 0;
      virtual void setRulePriority(double index, double priority) = 0;
      virtual bool getRuleStopIfTrue(double index) = 0;
      virtual void setRuleStopIfTrue(double index, bool stop) = 0;
      virtual double getRuleTimePeriod(double index) = 0;
      virtual void setRuleTimePeriod(double index, double period) = 0;
      virtual double getRuleRank(double index) = 0;
      virtual void setRuleRank(double index, double rank) = 0;
      virtual double getRuleStdDev(double index) = 0;
      virtual void setRuleStdDev(double index, double stdDev) = 0;
      virtual bool getRuleAboveAverage(double index) = 0;
      virtual void setRuleAboveAverage(double index, bool set) = 0;
      virtual bool getRulePercent(double index) = 0;
      virtual void setRulePercent(double index, bool set) = 0;
      virtual bool getRuleBottom(double index) = 0;
      virtual void setRuleBottom(double index, bool set) = 0;
      virtual bool getRuleEqualAverage(double index) = 0;
      virtual void setRuleEqualAverage(double index, bool set) = 0;
      virtual std::string summary() = 0;

    protected:
      void loadHybridMethods() override {}

    protected:
      static constexpr auto TAG = "XlsxConditionalFormat";
  };

} // namespace margelo::nitro::xlsx
