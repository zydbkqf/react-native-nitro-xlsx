import { type HybridObject, type AnyMap } from 'react-native-nitro-modules'

export type CellType = 'empty' | 'string' | 'number' | 'boolean' | 'date' | 'error' | 'formula' | 'blank'

export interface XlsxWorkbook extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  addWorksheet(name?: string): XlsxWorksheet
  getWorksheet(index: number): XlsxWorksheet
  getWorksheetByName(name: string): XlsxWorksheet
  getOrAddWorksheet(name: string): XlsxWorksheet
  getWorksheetCount(): number
  deleteSheet(name: string): void
  updateSheetName(oldName: string, newName: string): void
  clone(existingName: string, newName: string): XlsxWorksheet
  addCellFormat(): XlsxCellFormat
  getBuffer(): Promise<ArrayBuffer>
  toJSON(): Record<string, Array<AnyMap>>
}

export interface XlsxWorksheet extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  // Basic write methods
  writeString(row: number, col: number, value: string, format?: XlsxCellFormat): void
  writeNumber(row: number, col: number, value: number, format?: XlsxCellFormat): void
  writeBoolean(row: number, col: number, value: boolean, format?: XlsxCellFormat): void
  writeBlank(row: number, col: number, format?: XlsxCellFormat): void

  // Formula support
  writeFormula(row: number, col: number, formula: string, format?: XlsxCellFormat): void
  writeFormulaNum(row: number, col: number, formula: string, number: number, format?: XlsxCellFormat): void
  writeFormulaString(row: number, col: number, formula: string, str: string, format?: XlsxCellFormat): void
  writeFormulaBoolean(row: number, col: number, formula: string, boolVal: boolean, format?: XlsxCellFormat): void
  writeArrayFormula(firstRow: number, firstCol: number, lastRow: number, lastCol: number, formula: string, format?: XlsxCellFormat): void

  // Date/DateTime support
  writeDatetime(row: number, col: number, datetime: number, format?: XlsxCellFormat): void
  writeURL(row: number, col: number, url: string, format?: XlsxCellFormat): void

  // Column/Row operations
  setColumn(firstCol: number, lastCol: number, width: number, format?: XlsxCellFormat): void
  setRow(row: number, height: number, format?: XlsxCellFormat): void
  deleteRow(row: number): boolean
  deleteColumn(col: number): boolean

  // Merge cells
  mergeRange(firstRow: number, firstCol: number, lastRow: number, lastCol: number, value: string, format?: XlsxCellFormat): void
  mergeRangeNum(firstRow: number, firstCol: number, lastRow: number, lastCol: number, value: number, format?: XlsxCellFormat): void
  unmergeCells(firstRow: number, firstCol: number, lastRow: number, lastCol: number): void

  // Tab appearance
  hide(): void
  activate(): void

  // Sheet protection
  protect(password?: string): void
  protectSheet(set?: boolean): void
  protectObjects(set?: boolean): void
  protectScenarios(set?: boolean): void

  // Password
  setPassword(password: string): void
  setPasswordHash(hash: string): void
  clearPassword(): void
  clearSheetProtection(): void

  // Fine-grained permissions (allow*/deny* pairs)
  allowInsertColumns(set?: boolean): void
  allowInsertRows(set?: boolean): void
  allowDeleteColumns(set?: boolean): void
  allowDeleteRows(set?: boolean): void
  allowSelectLockedCells(set?: boolean): void
  allowSelectUnlockedCells(set?: boolean): void

  denyInsertColumns(): void
  denyInsertRows(): void
  denyDeleteColumns(): void
  denyDeleteRows(): void
  denySelectLockedCells(): void
  denySelectUnlockedCells(): void

  // Protection state
  sheetProtected(): boolean
  objectsProtected(): boolean
  scenariosProtected(): boolean
  insertColumnsAllowed(): boolean
  insertRowsAllowed(): boolean
  deleteColumnsAllowed(): boolean
  deleteRowsAllowed(): boolean
  selectLockedCellsAllowed(): boolean
  selectUnlockedCellsAllowed(): boolean
  passwordIsSet(): boolean
  passwordHash(): string
  sheetProtectionSummary(): string

  // Outline/Grouping
  setColumnHidden(firstCol: number, lastCol: number, hidden: boolean): void
  setRowHidden(row: number, hidden: boolean): void

  // Comments (XLComments)
  setComment(row: number, col: number, text: string, authorId?: number): void
  getComment(row: number, col: number): string
  hasComment(row: number, col: number): boolean
  deleteComment(row: number, col: number): boolean
  getCommentCount(): number
  addCommentAuthor(author: string): number
  getCommentAuthor(authorId: number): string

  // Conditional formatting (XLConditionalFormats)
  getConditionalFormats(): XlsxConditionalFormats

  // Read methods
  getCellValue(row: number, col: number): string | number | boolean | null
  getCellString(row: number, col: number): string
  getCellRawValue(row: number, col: number): string
  getCellType(row: number, col: number): CellType
  getCellFormat(row: number, col: number): XlsxCellFormat
  hasFormula(row: number, col: number): boolean
  formula(row: number, col: number): string
  findCell(row: number, col: number): boolean
  getRowCount(): number
  getColumnCount(): number
  getLastRow(): number
  getLastColumn(): number
  getName(): string

  // JSON import / export for this worksheet
  toJSON(keys?: string[]): Array<AnyMap>
  fromJSON(data: Array<AnyMap>): void
}

/**
 * A single <conditionalFormatting> entry: a cell range (sqref) plus its cfRules.
 */
export interface XlsxConditionalFormat extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  getSqref(): string
  setSqref(sqref: string): void

  // Rule collection
  getRuleCount(): number
  createRule(): number
  deleteRule(index: number): boolean

  // Rule type / dxf
  getRuleType(index: number): number
  setRuleType(index: number, type: number): void
  getRuleDxfId(index: number): number
  setRuleDxfId(index: number, dxfId: number): void

  // Rule formula (supports up to 3 formulas: formula/formula2/formula3)
  getRuleFormula(index: number, formulaIndex: number): string
  setRuleFormula(index: number, formulaIndex: number, formula: string): void

  // Rule attributes
  getRuleOperator(index: number): number
  setRuleOperator(index: number, op: number): void
  getRuleText(index: number): string
  setRuleText(index: number, text: string): void
  getRulePriority(index: number): number
  setRulePriority(index: number, priority: number): void
  getRuleStopIfTrue(index: number): boolean
  setRuleStopIfTrue(index: number, stop: boolean): void
  getRuleTimePeriod(index: number): number
  setRuleTimePeriod(index: number, period: number): void
  getRuleRank(index: number): number
  setRuleRank(index: number, rank: number): void
  getRuleStdDev(index: number): number
  setRuleStdDev(index: number, stdDev: number): void
  getRuleAboveAverage(index: number): boolean
  setRuleAboveAverage(index: number, set: boolean): void
  getRulePercent(index: number): boolean
  setRulePercent(index: number, set: boolean): void
  getRuleBottom(index: number): boolean
  setRuleBottom(index: number, set: boolean): void
  getRuleEqualAverage(index: number): boolean
  setRuleEqualAverage(index: number, set: boolean): void

  summary(): string
}

/**
 * All <conditionalFormatting> entries of a worksheet.
 */
export interface XlsxConditionalFormats extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  getCount(): number
  create(sqref: string): XlsxConditionalFormat
  getByIndex(index: number): XlsxConditionalFormat
  summary(): string
}

export interface XlsxCellFormat extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
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

  // Read format properties (getters)
  getFontName(): string
  getFontSize(): number
  getFontColor(): number
  getIsBold(): boolean
  getIsItalic(): boolean
  getNumFormat(): string
  getBgColor(): number
}

export interface NitroXlsx extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  createWorkbook(): XlsxWorkbook
  openWorkbook(path: string): XlsxWorkbook
  openWorkbookFromBuffer(buffer: ArrayBuffer): XlsxWorkbook
  fromJSON(data: Record<string, Array<AnyMap>>): XlsxWorkbook
}
