import { type HybridObject } from 'react-native-nitro-modules'

export interface XlsxWorkbook extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  addWorksheet(name?: string): XlsxWorksheet
  getWorksheet(index: number): XlsxWorksheet
  getWorksheetByName(name: string): XlsxWorksheet
  getOrAddWorksheet(name: string): XlsxWorksheet
  getWorksheetCount(): number
  addFormat(): XlsxFormat
  getBuffer(): Promise<ArrayBuffer>
}

export interface XlsxWorksheet extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  // Basic write methods
  writeString(row: number, col: number, value: string, format?: XlsxFormat): void
  writeNumber(row: number, col: number, value: number, format?: XlsxFormat): void
  writeBoolean(row: number, col: number, value: boolean, format?: XlsxFormat): void
  writeBlank(row: number, col: number, format?: XlsxFormat): void
  
  // Formula support
  writeFormula(row: number, col: number, formula: string, format?: XlsxFormat): void
  writeFormulaNum(row: number, col: number, formula: string, number: number, format?: XlsxFormat): void
  writeFormulaString(row: number, col: number, formula: string, str: string, format?: XlsxFormat): void
  writeFormulaBoolean(row: number, col: number, formula: string, boolVal: boolean, format?: XlsxFormat): void
  writeArrayFormula(firstRow: number, firstCol: number, lastRow: number, lastCol: number, formula: string, format?: XlsxFormat): void
  
  // Date/DateTime support
  writeDatetime(row: number, col: number, datetime: number, format?: XlsxFormat): void
  writeURL(row: number, col: number, url: string, format?: XlsxFormat): void
  
  // Column/Row operations
  setColumn(firstCol: number, lastCol: number, width: number, format?: XlsxFormat): void
  setRow(row: number, height: number, format?: XlsxFormat): void
  
  // Merge cells
  mergeRange(firstRow: number, firstCol: number, lastRow: number, lastCol: number, value: string, format?: XlsxFormat): void
  mergeRangeNum(firstRow: number, firstCol: number, lastRow: number, lastCol: number, value: number, format?: XlsxFormat): void
  
  // Image (uses file path)
  insertImage(row: number, col: number, path: string, xOffset?: number, yOffset?: number, xScale?: number, yScale?: number): void
  insertImageBuffer(row: number, col: number, buffer: ArrayBuffer, extension: string, xOffset?: number, yOffset?: number, xScale?: number, yScale?: number): void
  
  // Autofilter
  autofilter(firstRow: number, firstCol: number, lastRow: number, lastCol: number): void
  
  // Freeze/Split panes
  freezePanes(row: number, col: number): void
  splitPanes(row: number, col: number): void
  
  // Tab appearance
  setTabColor(color: number): void
  hide(): void
  activate(): void
  setFirstSheet(): void
  protect(password?: string): void
  
  // Page setup
  setPortrait(): void
  setLandscape(): void
  setPaper(paperType: number): void
  setHeader(header: string): void
  setFooter(footer: string): void
  setPrintArea(firstRow: number, firstCol: number, lastRow: number, lastCol: number): void
  fitToPages(width: number, height: number): void
  setZoom(scale: number): void
  setGridlines(option: number): void
  centerHorizontally(): void
  centerVertically(): void
  
  // Outline/Grouping
  setOutline(level: number): void
  setColumnHidden(firstCol: number, lastCol: number, hidden: boolean): void
  setRowHidden(row: number, hidden: boolean): void
}

export interface XlsxFormat extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  // Font
  setFontName(name: string): void
  setFontSize(size: number): void
  setFontColor(color: number): void
  setFontScript(style: number): void
  setBold(): void
  setItalic(): void
  setUnderline(style?: number): void
  setStrikeout(): void
  
  // Alignment
  setAlign(align: number): void
  setVerticalAlign(align: number): void
  setTextWrap(): void
  setRotation(angle: number): void
  setIndent(level: number): void
  setShrink(): void
  
  // Number format
  setNumFormat(format: string): void
  setNumFormatIndex(index: number): void
  
  // Colors/Pattern
  setBgColor(color: number): void
  setFgColor(color: number): void
  setPattern(pattern: number): void
  
  // Borders
  setBorder(style: number): void
  setBorderColor(color: number): void
  setBottom(style: number): void
  setTop(style: number): void
  setLeft(style: number): void
  setRight(style: number): void
  setBottomColor(color: number): void
  setTopColor(color: number): void
  setLeftColor(color: number): void
  setRightColor(color: number): void
  setDiagonal(style: number): void
  setDiagonalColor(color: number): void
  
  // Protection
  setLocked(): void
  setUnlocked(): void
  
  // Other
  setHyperlink(): void
  setFontOnly(): void
}

export interface NitroXlsx extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  createWorkbook(): XlsxWorkbook
}