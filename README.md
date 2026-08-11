# react-native-nitro-xlsx

A React Native module for reading and writing Excel XLSX files using [OpenXLSX](https://github.com/troldal/OpenXLSX) via [react-native-nitro-modules](https://github.com/mrousavy/nitro).

## Features

- 📊 Full Excel XLSX file creation support
- 📖 Read existing XLSX files (from file path or buffer)
- 📝 Formula support (with default values)
- 📅 Date/DateTime support
- 🖼️ Image insertion (from file path or buffer)
- 🎨 Rich formatting (fonts, colors, borders, alignment)
- 📄 Page setup (margins, orientation, headers/footers)
- 🔒 Sheet protection
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

## API

### NitroXlsx

- `createWorkbook(tempDir?: string): XlsxWorkbook` - Create a new workbook. On Android, pass the app's cache directory (e.g., `RNFS.CachesDirectoryPath`) to avoid sandbox write permission issues.
- `openWorkbook(path: string): XlsxWorkbook` - Open workbook from file path
- `openWorkbookFromBuffer(buffer: ArrayBuffer): XlsxWorkbook` - Open workbook from buffer

### XlsxWorkbook

- `addWorksheet(name?: string): XlsxWorksheet` - Add a new worksheet
- `getWorksheet(index: number): XlsxWorksheet` - Get worksheet by index
- `getWorksheetByName(name: string): XlsxWorksheet` - Get worksheet by name
- `getOrAddWorksheet(name: string): XlsxWorksheet` - Get or add worksheet
- `getWorksheetCount(): number` - Get number of worksheets
- `addCellFormat(): XlsxCellFormat` - Add a new cell format
- `getBuffer(): Promise<ArrayBuffer>` - Generate and return the XLSX file as a buffer

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
| `writeDatetime(row, col, datetime, format?)` | Write a datetime (Excel serial date number) |
| `writeURL(row, col, url, format?)` | Write a hyperlink |

#### Column/Row
| Method | Description |
|--------|-------------|
| `setColumn(firstCol, lastCol, width, format?)` | Set column properties |
| `setRow(row, height, format?)` | Set row properties |
| `setColumnHidden(firstCol, lastCol, hidden)` | Hide/show columns |
| `setRowHidden(row, hidden)` | Hide/show row |

#### Merge
| Method | Description |
|--------|-------------|
| `mergeRange(firstRow, firstCol, lastRow, lastCol, value, format?)` | Merge cells with string value |
| `mergeRangeNum(firstRow, firstCol, lastRow, lastCol, value, format?)` | Merge cells with number value |

#### Image
| Method | Description |
|--------|-------------|
| `insertImage(row, col, path, xOffset?, yOffset?, xScale?, yScale?)` | Insert image from file |
| `insertImageBuffer(row, col, buffer, extension, xOffset?, yOffset?, xScale?, yScale?)` | Insert image from buffer |

#### View
| Method | Description |
|--------|-------------|
| `freezePanes(row, col)` | Freeze panes |
| `splitPanes(row, col)` | Split panes |
| `setTabColor(color)` | Set tab color |
| `hide()` | Hide worksheet |
| `activate()` | Activate worksheet |
| `setFirstSheet()` | Set as first sheet |
| `protect(password?)` | Protect worksheet |

#### Page Setup
| Method | Description |
|--------|-------------|
| `setPortrait()` | Set portrait orientation |
| `setLandscape()` | Set landscape orientation |
| `setPaper(paperType)` | Set paper size |
| `setHeader(header)` | Set header |
| `setFooter(footer)` | Set footer |
| `setPrintArea(firstRow, firstCol, lastRow, lastCol)` | Set print area |
| `fitToPages(width, height)` | Fit to pages |
| `setZoom(scale)` | Set zoom level |
| `setGridlines(option)` | Set gridlines |
| `centerHorizontally()` | Center horizontally |
| `centerVertically()` | Center vertically |

#### Read Methods
| Method | Description |
|--------|-------------|
| `getCellValue(row, col)` | Get cell value (string \| number \| boolean \| null) |
| `getCellString(row, col)` | Get cell value as string |
| `getCellRawValue(row, col)` | Get raw cell value |
| `getCellType(row, col)` | Get cell type |
| `getCellFormat(row, col)` | Get cell format |
| `getRowCount()` | Get total row count |
| `getColumnCount()` | Get total column count |
| `getLastRow()` | Get last row index |
| `getLastColumn()` | Get last column index |
| `getName()` | Get worksheet name |

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
- `number` - Numeric value
- `boolean` - Boolean value
- `date` - Date/time value
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
- `ImagePosition` - Image positioning options

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

- 🖼️ **Images**: Reading and writing images is not supported. Existing images in XLSX files will be ignored.
- 📊 **Charts**: Charts, graphs, and sparklines are not supported.
- ✏️ **Drawings/Shape**: Shapes, lines, arrows, and other drawing objects are not supported.
- 🔍 **Pivot Tables**: Pivot tables and pivot charts are not supported.
- 💻 **VBA/Macros**: VBA macros, forms, and ActiveX controls are not supported.
- 📋 **Comments**: Cell comments and notes are not supported.
- 📌 **Hyperlinks**: Only basic URL hyperlinks are supported.
- 🔄 **Conditional Formatting**: Conditional formatting rules are not supported.
- 📑 **Data Validation**: Data validation rules are not supported.

### Notes

- When reading XLSX files that contain unsupported features, the library will still read the cell data and formatting, but unsupported elements will be silently ignored.
- When writing XLSX files, unsupported features cannot be added and will be omitted from the output.