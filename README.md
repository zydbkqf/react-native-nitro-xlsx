# react-native-nitro-xlsx

A React Native module for creating Excel XLSX files using [libxlsxwriter](https://github.com/jmcnamara/libxlsxwriter) via [react-native-nitro-modules](https://github.com/mrousavy/nitro).

## Features

- 📊 Full Excel XLSX file creation support
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

### iOS

```sh
cd ios && pod install
```

### Android

No additional setup needed.

## Usage

```typescript
import { NitroXlsx, Align, Colors, NumFormat } from 'react-native-nitro-xlsx';

// Create a new workbook (no file path needed!)
const workbook = NitroXlsx.createWorkbook();

// Add a worksheet
const sheet = workbook.addWorksheet('Sheet1');

// Create a format for headers
const headerFormat = workbook.addFormat();
headerFormat.setBold();
headerFormat.setFontColor(Colors.WHITE);
headerFormat.setFgColor(Colors.BLUE);
headerFormat.setAlign(Align.CENTER);

// Write headers
sheet.writeString(0, 0, 'Name', headerFormat);
sheet.writeString(0, 1, 'Price', headerFormat);
sheet.writeString(0, 2, 'Total', headerFormat);

// Write data
sheet.writeString(1, 0, 'Apple');
sheet.writeNumber(1, 1, 1.50);
sheet.writeFormula(1, 2, '=B2*1.1');

sheet.writeString(2, 0, 'Orange');
sheet.writeNumber(2, 1, 2.00);
sheet.writeFormula(2, 2, '=B3*1.1');

// Date example
const date = new Date(2024, 0, 15);
const excelDate = (date.getTime() / 86400000) + 25569; // Convert to Excel date
sheet.writeDatetime(3, 0, excelDate);

// Set column widths
sheet.setColumn(0, 0, 15);
sheet.setColumn(1, 2, 12);

// Get the buffer and do something with it
const buffer = await workbook.getBuffer();

// Save to file system, share, upload, etc.
// Example: FileSystem.writeAsStringAsync(path, buffer.toString('base64'));
// Example: Share.open({ url: 'data:application/vnd.openxmlformats-officedocument.spreadsheetml.sheet;base64,' + btoa(buffer) });
```

## API

### NitroXlsx

- `createWorkbook(): XlsxWorkbook` - Create a new workbook

### XlsxWorkbook

- `addWorksheet(name?: string): XlsxWorksheet` - Add a new worksheet
- `addFormat(): XlsxFormat` - Add a new format
- `getBuffer(): Promise<ArrayBuffer>` - Generate and return the XLSX file as a buffer

### XlsxWorksheet

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

### XlsxFormat

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

MIT