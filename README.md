# react-native-nitro-xlsx

A React Native module for reading and writing Excel XLSX files using [OpenXLSX](https://github.com/troldal/OpenXLSX) via [react-native-nitro-modules](https://github.com/mrousavy/nitro).

## Features

- 📊 Full Excel XLSX file creation support
- 📖 Read existing XLSX files (from file path or buffer)
- 📝 Formula support (with default values) & array formulas
- 📅 Date / DateTime support
- 🔗 Hyperlinks (URLs)
- 🔄 JSON import / export (`fromJSON` / `toJSON`)
- 🎨 Rich formatting (fonts, colors, borders, alignment, number formats)
- 🔒 Sheet protection (password, objects/scenarios, fine-grained allow/deny)
- 📋 Cell comments (get / set / delete, multi-author)
- 🔤 Conditional formatting rules
- 🗂️ Sheet management (delete / rename / clone)
- ➕ Row/column delete, merge / unmerge
- 🧩 Hidden rows/columns
- 🔍 Read cells (value, type, format, formula) + findCell
- ⚠️ Structured native error codes mapped to JS
- 📦 Export as ArrayBuffer - no file system required!

## Installation

```sh
npm install react-native-nitro-xlsx
# or
yarn add react-native-nitro-xlsx
```

> **Note**: Native dependencies (OpenXlsx) are downloaded from GitHub during the first build. Please make sure you have a stable network connection.

### iOS

```sh
cd ios && pod install
```

### Android

No additional setup needed.

## Usage

> **Note**: Row and column indices are **1-based** (following Excel convention).

### Creating a Workbook

```typescript
import { NitroXlsx, Align, Colors, NumFormat } from 'react-native-nitro-xlsx';

// Create a new workbook (no file path needed!)
const workbook = NitroXlsx.createWorkbook();

// Add a worksheet
const sheet = workbook.addWorksheet('Sheet1');

// Create a cell format for headers
const headerFormat = workbook.addCellFormat();
headerFormat.setBold();
headerFormat.setFontColor(Colors.WHITE);
headerFormat.setFgColor(Colors.BLUE);
headerFormat.setAlign(Align.CENTER);

// Write headers
sheet.writeString(1, 1, 'Name', headerFormat);
sheet.writeString(1, 2, 'Price', headerFormat);
sheet.writeString(1, 3, 'Total', headerFormat);

// Write data
sheet.writeString(2, 1, 'Apple');
sheet.writeNumber(2, 2, 1.50);
sheet.writeFormula(2, 3, '=B2*1.1');

sheet.writeString(3, 1, 'Orange');
sheet.writeNumber(3, 2, 2.00);
sheet.writeFormula(3, 3, '=B3*1.1');

// Date example
const date = new Date(2024, 0, 15);
const excelDate = (date.getTime() / 86400000) + 25569; // Convert to Excel date
sheet.writeDatetime(4, 1, excelDate);

// Set column widths
sheet.setColumn(1, 1, 15);
sheet.setColumn(2, 3, 12);

// Get the buffer and do something with it
const buffer = await workbook.getBuffer();

// Save to file system, share, upload, etc.
// Example: FileSystem.writeAsStringAsync(path, buffer.toString('base64'));
// Example: Share.open({ url: 'data:application/vnd.openxmlformats-officedocument.spreadsheetml.sheet;base64,' + btoa(buffer) });
```

### Reading a Workbook

```typescript
import { NitroXlsx } from 'react-native-nitro-xlsx';

// Open workbook from file path
const workbook = NitroXlsx.openWorkbook('/path/to/file.xlsx');

// Or open from buffer
// const workbook = NitroXlsx.openWorkbookFromBuffer(arrayBuffer);

// Get worksheet
const sheet = workbook.getWorksheet(0);
const sheetByName = workbook.getWorksheetByName('Sheet1');

// Get worksheet info
const name = sheet.getName();
const lastRow = sheet.getLastRow();
const lastCol = sheet.getLastColumn();

// Read cell values
for (let row = 1; row <= lastRow; row++) {
  for (let col = 1; col <= lastCol; col++) {
    const value = sheet.getCellValue(row, col);
    const type = sheet.getCellType(row, col);
    const strValue = sheet.getCellString(row, col);
    console.log(`Cell [${row},${col}]: ${value} (type: ${type})`);
  }
}

// Read cell format
const format = sheet.getCellFormat(1, 1);
const isBold = format.getIsBold();
const fontName = format.getFontName();
const fontSize = format.getFontSize();
```

### Working with JSON

Per-sheet conversion lives on `XlsxWorksheet`. Workbook-level conversion uses a
`Record` keyed by worksheet name.

#### Export a worksheet (`sheet.toJSON`)

Convert one worksheet's data to an array of records. By default, the first row is used as object keys.

```typescript
const sheet = workbook.getWorksheetByName('Data');

// Use the first row as keys (default)
const rows = sheet.toJSON();
// Result: [{ "Name": "Alice", "Age": 30 }, { "Name": "Bob", "Age": 25 }]

// Provide custom keys
const rows2 = sheet.toJSON(['firstName', 'yearsOld']);
// Result: [{ "firstName": "Alice", "yearsOld": 30 }, ...]
```

#### Import into a worksheet (`sheet.fromJSON`)

Write an array of records into an existing worksheet. The first record's keys become the header row.

```typescript
const sheet = workbook.addWorksheet('Data');
sheet.fromJSON([
  { Name: 'Alice', Age: 30, Active: true },
  { Name: 'Bob', Age: 25, Active: false },
]);
// Row 1 (header): Active | Age | Name   (keys are sorted alphabetically)
// Row 2:          true   | 30  | Alice
// Row 3:          false  | 25  | Bob
```

#### Export all sheets (`workbook.toJSON`)

Returns a `Record` keyed by worksheet name.

```typescript
const all = workbook.toJSON();
// Result: { "Data": [{ Name: 'Alice', ... }], "Summary": [{ ... }] }
```

#### Create a workbook from multiple sheets (`NitroXlsx.fromJSON`)

Each key becomes a worksheet.

```typescript
const workbook = NitroXlsx.fromJSON({
  Data: [{ Name: 'Alice', Age: 30 }],
  Summary: [{ Total: 1 }],
});
// Creates worksheets "Data" and "Summary"

const buffer = await workbook.getBuffer();
```

## API

### NitroXlsx

- `createWorkbook(): XlsxWorkbook` - Create a new workbook. The native side supplies the platform cache directory automatically (`Context.getCacheDir()` on Android, `NSCachesDirectory` on iOS).
- `openWorkbook(path: string): XlsxWorkbook` - Open workbook from file path
- `openWorkbookFromBuffer(buffer: ArrayBuffer): XlsxWorkbook` - Open workbook from buffer
- `fromJSON(data: Record<string, Array<Record<string, string | number | boolean | null>>>): XlsxWorkbook` - Create a workbook with one worksheet per key (native `AnyMap`s, no JSON round-trip)

### XlsxWorkbook

- `addWorksheet(name?: string): XlsxWorksheet` - Add a new worksheet
- `getWorksheet(index: number): XlsxWorksheet` - Get worksheet by index
- `getWorksheetByName(name: string): XlsxWorksheet` - Get worksheet by name
- `getOrAddWorksheet(name: string): XlsxWorksheet` - Get or add worksheet
- `getWorksheetCount(): number` - Get number of worksheets
- `deleteSheet(name: string): void` - Delete a worksheet by name
- `updateSheetName(oldName: string, newName: string): void` - Rename a worksheet and rewrite formula references to it
- `clone(existingName: string, newName: string): XlsxWorksheet` - Copy a worksheet
- `addCellFormat(): XlsxCellFormat` - Add a new cell format
- `getBuffer(): Promise<ArrayBuffer>` - Generate and return the XLSX file as a buffer
- `toJSON(): Record<string, Array<Record<string, string | number | boolean | null>>>` - Export every worksheet as a `Record` keyed by sheet name

### XlsxWorksheet

> **Note**: All row and column indices are **1-based** (following Excel convention).

#### Basic Write Methods
| Method | Description |
|--------|-------------|
| `writeString(row, col, value, format?)` | Write a string |
| `writeNumber(row, col, value, format?)` | Write a number |
| `writeBoolean(row, col, value, format?)` | Write a boolean |
| `writeBlank(row, col, format?)` | Write a blank cell |

#### Formula Support
| Method | Description |
|--------|-------------|
| `writeFormula(row, col, formula, format?)` | Write a formula |
| `writeFormulaNum(row, col, formula, number, format?)` | Write a formula with number default |
| `writeFormulaString(row, col, formula, string, format?)` | Write a formula with string default |
| `writeFormulaBoolean(row, col, formula, boolean, format?)` | Write a formula with boolean default |
| `writeArrayFormula(firstRow, firstCol, lastRow, lastCol, formula, format?)` | Write an array formula |

#### Date/URL
| Method | Description |
|--------|-------------|
| `writeDatetime(row, col, datetime, format?)` | Write a datetime (1900-based Excel serial date number; see Cell Types) |
| `writeURL(row, col, url, format?)` | Write a hyperlink |

#### Column/Row
| Method | Description |
|--------|-------------|
| `setColumn(firstCol, lastCol, width, format?)` | Set column properties |
| `setRow(row, height, format?)` | Set row properties |
| `deleteRow(row)` | Delete a row (returns `true` if a row entry existed and was removed) |
| `deleteColumn(col)` | Delete a column and shift remaining columns left (returns `true` on success) |
| `setColumnHidden(firstCol, lastCol, hidden)` | Hide/show columns |
| `setRowHidden(row, hidden)` | Hide/show row |

#### Merge
| Method | Description |
|--------|-------------|
| `mergeRange(firstRow, firstCol, lastRow, lastCol, value, format?)` | Merge cells with string value |
| `mergeRangeNum(firstRow, firstCol, lastRow, lastCol, value, format?)` | Merge cells with number value |
| `unmergeCells(firstRow, firstCol, lastRow, lastCol)` | Unmerge a previously merged range |

#### Comments
| Method | Description |
|--------|-------------|
| `setComment(row, col, text, authorId?)` | Set a cell comment (creates the comments part on first use) |
| `getComment(row, col)` | Get comment text (empty string if none) |
| `hasComment(row, col)` | Whether the cell has a comment |
| `deleteComment(row, col)` | Delete a cell comment (returns `true` if one existed) |
| `getCommentCount()` | Number of comments on this sheet |
| `addCommentAuthor(author)` | Register an author, returns its `authorId` |
| `getCommentAuthor(authorId)` | Get author name by id |

#### View

| Method | Description |
|--------|-------------|
| `hide()` | Hide worksheet |
| `activate()` | Activate worksheet |

#### Sheet Protection
| Method | Description |
|--------|-------------|
| `protect(password?)` | Convenience: set optional password and enable sheet protection |
| `protectSheet(set?)` | Enable/disable sheet protection (`set` defaults to `true`) |
| `protectObjects(set?)` | Protect drawing objects |
| `protectScenarios(set?)` | Protect scenarios |
| `setPassword(password)` | Set password (hashed by Excel algorithm) |
| `setPasswordHash(hash)` | Set a pre-computed 4-digit hex password hash |
| `clearPassword()` | Remove the password attribute |
| `clearSheetProtection()` | Remove `<sheetProtection>` entirely |

Fine-grained permissions (only meaningful while `protectSheet` is on).
`allow*(set?)` defaults to `true`; `deny*()` is shorthand for `allow*(false)`.

| Allow | Deny | Effect when allowed |
|-------|------|---------------------|
| `allowInsertColumns(set?)` | `denyInsertColumns()` | User may insert columns |
| `allowInsertRows(set?)` | `denyInsertRows()` | User may insert rows |
| `allowDeleteColumns(set?)` | `denyDeleteColumns()` | User may delete columns |
| `allowDeleteRows(set?)` | `denyDeleteRows()` | User may delete rows |
| `allowSelectLockedCells(set?)` | `denySelectLockedCells()` | User may select locked cells |
| `allowSelectUnlockedCells(set?)` | `denySelectUnlockedCells()` | User may select unlocked cells |

Protection state getters:

| Method | Returns |
|--------|---------|
| `sheetProtected()` / `objectsProtected()` / `scenariosProtected()` | Whether the corresponding protection is on |
| `insertColumnsAllowed()` / `insertRowsAllowed()` | Whether insert is allowed despite protection |
| `deleteColumnsAllowed()` / `deleteRowsAllowed()` | Whether delete is allowed despite protection |
| `selectLockedCellsAllowed()` / `selectUnlockedCellsAllowed()` | Whether select is allowed despite protection |
| `passwordIsSet()` | Whether a password hash is present |
| `passwordHash()` | The stored password hash (empty if none) |
| `sheetProtectionSummary()` | Human-readable summary of the protection settings |

Example:

```typescript
const sheet = workbook.addWorksheet('Locked');
sheet.writeString(1, 1, 'Protected');
sheet.setPassword('s3cret');
sheet.protectSheet();
sheet.allowDeleteRows();       // users may still delete rows
sheet.denySelectLockedCells(); // users cannot select locked cells
console.log(sheet.sheetProtected());           // true
console.log(sheet.deleteRowsAllowed());        // true
console.log(sheet.selectLockedCellsAllowed()); // false
```

#### Read Methods
| Method | Description |
|--------|-------------|
| `getCellValue(row, col)` | Get cell value (string \| number \| boolean \| null) |
| `getCellString(row, col)` | Get cell value as string |
| `getCellRawValue(row, col)` | Get raw cell value |
| `getCellType(row, col)` | Get cell type |
| `getCellFormat(row, col)` | Get cell format |
| `hasFormula(row, col)` | Whether the cell contains a formula |
| `formula(row, col)` | Get the cell's formula string (empty if none) |
| `findCell(row, col)` | Whether a cell exists at these coordinates (does **not** create it) |
| `getRowCount()` | Get total row count |
| `getColumnCount()` | Get total column count |
| `getLastRow()` | Get last row index |
| `getLastColumn()` | Get last column index |
| `getName()` | Get worksheet name |
| `toJSON(keys?)` | Export this worksheet as an array of records (`keys` defaults to first-row headers) |
| `fromJSON(data)` | Write an array of records into this worksheet (first row = header) |

#### Conditional Formatting
| Method | Description |
|--------|-------------|
| `getConditionalFormats()` | Get the worksheet's `XlsxConditionalFormats` collection |

##### XlsxConditionalFormats
| Method | Description |
|--------|-------------|
| `getCount()` | Number of `<conditionalFormatting>` entries |
| `create(sqref)` | Create a new entry for a range (e.g. `"A1:A10"`), returns `XlsxConditionalFormat` |
| `getByIndex(index)` | Get an entry by index |
| `summary()` | Debug summary string |

##### XlsxConditionalFormat
| Method | Description |
|--------|-------------|
| `getSqref()` / `setSqref(sqref)` | The range these rules apply to |
| `getRuleCount()` / `createRule()` | Manage `cfRule` entries (`createRule` returns the new index) |
| `getRuleType(index)` / `setRuleType(index, type)` | Rule type — use `CfType` constants |
| `getRuleDxfId(index)` / `setRuleDxfId(index, dxfId)` | Differential format id |
| `getRuleFormula(index, formulaIndex)` / `setRuleFormula(index, formulaIndex, formula)` | Rule formula (`formulaIndex` must be `0`; OpenXLSX supports one formula per rule) |
| `getRuleOperator(index)` / `setRuleOperator(index, op)` | Comparison operator — use `CfOperator` |
| `getRuleText(index)` / `setRuleText(index, text)` | Text for text-based rules |
| `getRulePriority(index)` / `setRulePriority(index, priority)` | Rule priority (1-based) |
| `getRuleStopIfTrue(index)` / `setRuleStopIfTrue(index, stop)` | Stop evaluating further rules |
| `getRuleTimePeriod(index)` / `setRuleTimePeriod(index, period)` | Time period — use `CfTimePeriod` |
| `getRuleRank(index)` / `setRuleRank(index, rank)` | Top/bottom N |
| `getRuleStdDev(index)` / `setRuleStdDev(index, stdDev)` | Standard deviations for aboveAverage |
| `getRuleAboveAverage` / `setRuleAboveAverage` | aboveAverage flag |
| `getRulePercent` / `setRulePercent` | percent flag |
| `getRuleBottom` / `setRuleBottom` | bottom flag |
| `getRuleEqualAverage` / `setRuleEqualAverage` | equalAverage flag |
| `summary()` | Debug summary string |

Example:

```typescript
const cf = sheet.getConditionalFormats().create('B2:B100');
const ruleIndex = cf.createRule();
cf.setRuleType(ruleIndex, CfType.CELL_IS);
cf.setRuleOperator(ruleIndex, CfOperator.GREATER_THAN);
cf.setRuleFormula(ruleIndex, 0, '100');
cf.setRuleDxfId(ruleIndex, 0);
cf.setRulePriority(ruleIndex, 1);
```

### XlsxCellFormat

The `XlsxCellFormat` interface is based on OpenXLSX's `XLCellFormat` class and provides rich cell formatting capabilities.

#### Font
- `setFontName(name)`, `setFontSize(size)`, `setFontColor(color)`
- `setBold()`, `setItalic()`, `setUnderline(style?)`, `setStrikeout()`
- `setFontScript(style)` - Superscript/subscript

#### Alignment
- `setAlign(align)`, `setVerticalAlign(align)`
- `setTextWrap()`, `setRotation(angle)`
- `setIndent(level)`, `setShrink()`

#### Number Format
- `setNumFormat(format)` - Set custom format string
- `setNumFormatIndex(index)` - Set built-in format (use `NumFormat` constants)

#### Colors/Pattern
- `setBgColor(color)`, `setFgColor(color)`
- `setPattern(pattern)`

#### Borders
- `setBorder(style)`, `setBorderColor(color)`
- `setTop/Botton/Left/Right(style)`, `setTopColor/BottomColor/LeftColor/RightColor(color)`
- `setDiagonal(style)`, `setDiagonalColor(color)`

#### Protection
- `setLocked()`, `setUnlocked()`

#### Other
- `setHyperlink()`, `setFontOnly()`

#### Read Properties
- `getFontName()` - Get font name
- `getFontSize()` - Get font size
- `getFontColor()` - Get font color
- `getIsBold()` - Check if bold
- `getIsItalic()` - Check if italic
- `getNumFormat()` - Get number format
- `getBgColor()` - Get background color

## Cell Types

When reading cells, the following types are returned:

- `empty` - Empty cell
- `string` - Text string
- `number` - Numeric value. Dates are stored as **Excel serial numbers in the 1900 date system** (days since `1900-01-01`, where `1900-01-01` = `1`, including the spurious `1900-02-29`), and are read back as `number`:
  ```ts
  // Excel serial (1900-based) → JS Date
  const date = new Date(Math.round((serial - 25569) * 86400000));
  // JS Date → Excel serial (1900-based)
  const serial = date.getTime() / 86400000 + 25569;
  ```
- `boolean` - Boolean value
- `error` - Error value
- `formula` - Formula
- `blank` - Blank cell

## Constants

The library exports several useful constants:

- `Align` - Cell alignment values
- `VAlign` - Vertical alignment values
- `Border` - Border styles
- `Underline` - Underline styles
- `Script` - Superscript/subscript
- `Pattern` - Fill patterns
- `Paper` - Paper sizes
- `Gridlines` - Gridline options
- `Colors` - Common RGB colors
- `NumFormat` - Built-in number format indices
- `CfType` - Conditional formatting rule types
- `CfOperator` - Conditional formatting comparison operators
- `CfTimePeriod` - Conditional formatting time periods
- `XlsxErrorCode` - Native error codes

## Error Handling

Native failures are thrown as JS `Error`s whose message is `methodName: CODE: detail`.
Convert them to a structured `XlsxError` with `XlsxError.from`:

```typescript
import { NitroXlsx, XlsxError, XlsxErrorCode } from 'react-native-nitro-xlsx';

try {
  workbook.deleteSheet('Missing');
} catch (e) {
  const err = XlsxError.from(e);
  if (err.code === XlsxErrorCode.SHEET_NOT_FOUND) {
    // handle missing sheet
  }
}
```

Error codes:

| Code | Meaning |
|------|---------|
| `INVALID_ARGUMENT` | Bad row/col/name or other input |
| `SHEET_NOT_FOUND` | Worksheet does not exist |
| `SHEET_EXISTS` | Worksheet name already taken |
| `INDEX_OUT_OF_RANGE` | Row/col/index out of range |
| `CELL_NOT_FOUND` | Referenced cell does not exist |
| `WORKBOOK_CLOSED` | Workbook already finalized via `getBuffer()` |
| `IO_ERROR` | File open/save failure |
| `UNSUPPORTED` | Operation not supported by the underlying engine |
| `XLSX_ERROR` | Generic OpenXLSX failure |
| `INTERNAL_ERROR` | Unexpected native error |

## Development

To regenerate the native specs after modifying the TypeScript spec files:

```sh
npx nitrogen
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Third-party open source attributions are listed in the [NOTICE](NOTICE) file.

## Acknowledgements

This library wraps the excellent [OpenXLSX](https://github.com/troldal/OpenXLSX) library, a modern C++ library for reading and writing Excel XLSX files.

## Known Limitations

This library is based on OpenXLSX and inherits the following limitations:

### Unsupported Formats

- **XLS format**: Only XLSX (Office Open XML) format is supported. Legacy XLS (Binary) format is not supported.

### Unsupported Features

The following Excel features are not supported and will be ignored when reading or writing files:

- 📊 **Charts**: Charts, graphs, and sparklines are not supported.
- 🖼️ **Drawings/Images/Shapes**: Images, shapes, lines, arrows and other drawing objects fall under OpenXLSX's drawing support, which is not implemented. Existing images in XLSX files are silently ignored.
- 🔍 **Pivot Tables**: Pivot tables and pivot charts are not supported.
- 💻 **VBA/Macros**: VBA macros, forms, and ActiveX controls are not supported.
- 📌 **Hyperlinks**: Only basic URL hyperlinks are supported.
- 📑 **Data Validation**: Data validation rules are not supported.

### Notes

- When reading XLSX files that contain unsupported features, the library will still read the cell data and formatting, but unsupported elements will be silently ignored.
- When writing XLSX files, unsupported features cannot be added and will be omitted from the output.
- `deleteColumn` shifts cell values and formats left; formula references are not rewritten (same limitation as OpenXLSX's `deleteRow`).
- Conditional formatting supports one formula per `cfRule` (OpenXLSX limitation). `colorScale` / `dataBar` / `iconSet` are not supported.
- `deleteRule` on `XlsxConditionalFormat` always returns `false` (OpenXLSX does not support removing a `cfRule`).
- Conditional formatting rule `type` / `operator` / `timePeriod` are direct numeric mappings of OpenXLSX enums; passing an out-of-range value results in undefined behavior. Use the exported `CfType` / `CfOperator` / `CfTimePeriod` constants.
- `setComment` automatically registers an author so the generated `comments.xml` always has a valid `authorId` (verified to open cleanly in Excel/WPS).
- `getComment` returns an empty string for a cell without a comment (it does not throw). Use `hasComment` first if you need to distinguish.