#pragma once

#include <NitroModules/AnyMap.hpp>
#include <OpenXLSX/XLSheet.hpp>
#include <OpenXLSX/XLWorkbook.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace margelo::nitro::xlsx {

/**
 * Convert one worksheet to an array of records.
 * @param keys if provided, used as record keys; otherwise row 1 is treated as the header
 */
std::vector<std::shared_ptr<AnyMap>> worksheetToRecords(
    OpenXLSX::XLWorksheet& worksheet,
    const std::optional<std::vector<std::string>>& keys = std::nullopt);

/**
 * Write an array of records into a worksheet.
 * The first record's keys become the header row; later-only keys extend the columns.
 */
void recordsToWorksheet(
    OpenXLSX::XLWorksheet& worksheet,
    const std::vector<std::shared_ptr<AnyMap>>& data);

/**
 * Export every worksheet as a Record keyed by sheet name.
 */
std::unordered_map<std::string, std::vector<std::shared_ptr<AnyMap>>> workbookToRecords(
    OpenXLSX::XLWorkbook& workbook);

} // namespace margelo::nitro::xlsx
