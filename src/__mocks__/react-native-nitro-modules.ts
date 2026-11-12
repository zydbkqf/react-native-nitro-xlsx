// Manual mock for `react-native-nitro-modules` (mapped via jest.config.js moduleNameMapper).
// `NitroModules.createHybridObject` hands out typed fakes whose methods are `jest.fn`s with
// realistic behavior, so tests can assert on forwarded arguments (`mock.calls`) and return shapes.
import type {
  CellType,
  NitroXlsx,
  XlsxCellFormat,
  XlsxConditionalFormat,
  XlsxConditionalFormats,
  XlsxWorkbook,
  XlsxWorksheet,
} from '../specs/NitroXlsx.nitro'

export type HybridObject<T> = { __platforms?: T }

export type AnyMap = Record<string, unknown>

type RowJSON = Record<string, string | number | boolean | null>

interface CellState {
  value: string | number | boolean | null
  type: CellType
  formula?: string
}

interface SheetState {
  name: string
  cells: Map<string, CellState>
  comments: Map<string, string>
  commentAuthors: string[]
}

interface RuleState {
  type: number
  dxfId: number
  formulas: string[]
  operator: number
  text: string
  priority: number
  stopIfTrue: boolean
  timePeriod: number
  rank: number
  stdDev: number
  aboveAverage: boolean
  percent: boolean
  bottom: boolean
  equalAverage: boolean
}

const cellKey = (row: number, col: number): string => `${row}:${col}`

function rowsFromCells(cells: Map<string, CellState>): ReturnType<XlsxWorksheet['toJSON']> {
  const rows = new Map<number, RowJSON>()
  for (const [key, cell] of cells) {
    const sep = key.indexOf(':')
    const row = Number(key.slice(0, sep))
    const col = key.slice(sep + 1)
    const entry = rows.get(row) ?? {}
    entry[col] = cell.value
    rows.set(row, entry)
  }
  return [...rows.entries()]
    .sort((a, b) => a[0] - b[0])
    .map(([, values]) => values)
}

function createFakeCellFormat(): XlsxCellFormat {
  const state = {
    fontName: 'Calibri',
    fontSize: 11,
    fontColor: 0,
    bgColor: 0xffffff,
    bold: false,
    italic: false,
    numFormat: 'General',
  }
  const cellFormat: XlsxCellFormat = {
    name: 'CellFormat',
    toString: jest.fn((): string => '[HybridObject CellFormat]'),
    equals: jest.fn((other: unknown): boolean => other === cellFormat),
    dispose: jest.fn((): void => undefined),

    setFontName: jest.fn((fontName: string): void => {
      state.fontName = fontName
    }),
    setFontSize: jest.fn((fontSize: number): void => {
      state.fontSize = fontSize
    }),
    setFontColor: jest.fn((fontColor: number): void => {
      state.fontColor = fontColor
    }),
    setFontScript: jest.fn((): void => undefined),
    setBold: jest.fn((): void => {
      state.bold = true
    }),
    setItalic: jest.fn((): void => {
      state.italic = true
    }),
    setUnderline: jest.fn((): void => undefined),
    setStrikeout: jest.fn((): void => undefined),

    setAlign: jest.fn((): void => undefined),
    setVerticalAlign: jest.fn((): void => undefined),
    setTextWrap: jest.fn((): void => undefined),
    setRotation: jest.fn((): void => undefined),
    setIndent: jest.fn((): void => undefined),
    setShrink: jest.fn((): void => undefined),

    setNumFormat: jest.fn((format: string): void => {
      state.numFormat = format
    }),
    setNumFormatIndex: jest.fn((index: number): void => {
      state.numFormat = `#${index}`
    }),

    setBgColor: jest.fn((bgColor: number): void => {
      state.bgColor = bgColor
    }),
    setFgColor: jest.fn((): void => undefined),
    setPattern: jest.fn((): void => undefined),

    setBorder: jest.fn((): void => undefined),
    setBorderColor: jest.fn((): void => undefined),
    setBottom: jest.fn((): void => undefined),
    setTop: jest.fn((): void => undefined),
    setLeft: jest.fn((): void => undefined),
    setRight: jest.fn((): void => undefined),
    setBottomColor: jest.fn((): void => undefined),
    setTopColor: jest.fn((): void => undefined),
    setLeftColor: jest.fn((): void => undefined),
    setRightColor: jest.fn((): void => undefined),
    setDiagonal: jest.fn((): void => undefined),
    setDiagonalColor: jest.fn((): void => undefined),

    setLocked: jest.fn((): void => undefined),
    setUnlocked: jest.fn((): void => undefined),

    setHyperlink: jest.fn((): void => undefined),
    setFontOnly: jest.fn((): void => undefined),

    getFontName: jest.fn((): string => state.fontName),
    getFontSize: jest.fn((): number => state.fontSize),
    getFontColor: jest.fn((): number => state.fontColor),
    getIsBold: jest.fn((): boolean => state.bold),
    getIsItalic: jest.fn((): boolean => state.italic),
    getNumFormat: jest.fn((): string => state.numFormat),
    getBgColor: jest.fn((): number => state.bgColor),
  }
  return cellFormat
}

function createFakeConditionalFormat(initialSqref: string): XlsxConditionalFormat {
  let sqref = initialSqref
  const rules: RuleState[] = []
  const ruleAt = (index: number): RuleState => {
    const rule = rules[index]
    if (!rule) throw new Error(`getRule: INDEX_OUT_OF_RANGE: no rule at index ${index}`)
    return rule
  }
  const conditionalFormat: XlsxConditionalFormat = {
    name: 'ConditionalFormat',
    toString: jest.fn((): string => '[HybridObject ConditionalFormat]'),
    equals: jest.fn((other: unknown): boolean => other === conditionalFormat),
    dispose: jest.fn((): void => undefined),

    getSqref: jest.fn((): string => sqref),
    setSqref: jest.fn((next: string): void => {
      sqref = next
    }),

    getRuleCount: jest.fn((): number => rules.length),
    createRule: jest.fn((): number => {
      rules.push({
        type: 0,
        dxfId: -1,
        formulas: ['', '', ''],
        operator: 0,
        text: '',
        priority: rules.length,
        stopIfTrue: false,
        timePeriod: 0,
        rank: 0,
        stdDev: 0,
        aboveAverage: false,
        percent: false,
        bottom: false,
        equalAverage: false,
      })
      return rules.length - 1
    }),
    deleteRule: jest.fn((index: number): boolean => {
      if (index < 0 || index >= rules.length) return false
      rules.splice(index, 1)
      return true
    }),

    getRuleType: jest.fn((index: number): number => ruleAt(index).type),
    setRuleType: jest.fn((index: number, type: number): void => {
      ruleAt(index).type = type
    }),
    getRuleDxfId: jest.fn((index: number): number => ruleAt(index).dxfId),
    setRuleDxfId: jest.fn((index: number, dxfId: number): void => {
      ruleAt(index).dxfId = dxfId
    }),

    getRuleFormula: jest.fn((index: number, formulaIndex: number): string => ruleAt(index).formulas[formulaIndex] ?? ''),
    setRuleFormula: jest.fn((index: number, formulaIndex: number, formula: string): void => {
      if (formulaIndex >= 0 && formulaIndex < 3) ruleAt(index).formulas[formulaIndex] = formula
    }),

    getRuleOperator: jest.fn((index: number): number => ruleAt(index).operator),
    setRuleOperator: jest.fn((index: number, op: number): void => {
      ruleAt(index).operator = op
    }),
    getRuleText: jest.fn((index: number): string => ruleAt(index).text),
    setRuleText: jest.fn((index: number, text: string): void => {
      ruleAt(index).text = text
    }),
    getRulePriority: jest.fn((index: number): number => ruleAt(index).priority),
    setRulePriority: jest.fn((index: number, priority: number): void => {
      ruleAt(index).priority = priority
    }),
    getRuleStopIfTrue: jest.fn((index: number): boolean => ruleAt(index).stopIfTrue),
    setRuleStopIfTrue: jest.fn((index: number, stop: boolean): void => {
      ruleAt(index).stopIfTrue = stop
    }),
    getRuleTimePeriod: jest.fn((index: number): number => ruleAt(index).timePeriod),
    setRuleTimePeriod: jest.fn((index: number, period: number): void => {
      ruleAt(index).timePeriod = period
    }),
    getRuleRank: jest.fn((index: number): number => ruleAt(index).rank),
    setRuleRank: jest.fn((index: number, rank: number): void => {
      ruleAt(index).rank = rank
    }),
    getRuleStdDev: jest.fn((index: number): number => ruleAt(index).stdDev),
    setRuleStdDev: jest.fn((index: number, stdDev: number): void => {
      ruleAt(index).stdDev = stdDev
    }),
    getRuleAboveAverage: jest.fn((index: number): boolean => ruleAt(index).aboveAverage),
    setRuleAboveAverage: jest.fn((index: number, set: boolean): void => {
      ruleAt(index).aboveAverage = set
    }),
    getRulePercent: jest.fn((index: number): boolean => ruleAt(index).percent),
    setRulePercent: jest.fn((index: number, set: boolean): void => {
      ruleAt(index).percent = set
    }),
    getRuleBottom: jest.fn((index: number): boolean => ruleAt(index).bottom),
    setRuleBottom: jest.fn((index: number, set: boolean): void => {
      ruleAt(index).bottom = set
    }),
    getRuleEqualAverage: jest.fn((index: number): boolean => ruleAt(index).equalAverage),
    setRuleEqualAverage: jest.fn((index: number, set: boolean): void => {
      ruleAt(index).equalAverage = set
    }),

    summary: jest.fn((): string => `ConditionalFormat(${sqref}, rules=${rules.length})`),
  }
  return conditionalFormat
}

function createFakeConditionalFormats(): XlsxConditionalFormats {
  const formats: XlsxConditionalFormat[] = []
  const conditionalFormats: XlsxConditionalFormats = {
    name: 'ConditionalFormats',
    toString: jest.fn((): string => '[HybridObject ConditionalFormats]'),
    equals: jest.fn((other: unknown): boolean => other === conditionalFormats),
    dispose: jest.fn((): void => undefined),

    getCount: jest.fn((): number => formats.length),
    create: jest.fn((sqref: string): XlsxConditionalFormat => {
      const format = createFakeConditionalFormat(sqref)
      formats.push(format)
      return format
    }),
    getByIndex: jest.fn((index: number): XlsxConditionalFormat => {
      const format = formats[index]
      if (!format) throw new Error(`getByIndex: INDEX_OUT_OF_RANGE: no conditional format at index ${index}`)
      return format
    }),
    summary: jest.fn((): string => `ConditionalFormats(count=${formats.length})`),
  }
  return conditionalFormats
}

function createFakeWorksheet(state: SheetState): XlsxWorksheet {
  const conditionalFormats = createFakeConditionalFormats()
  const cellAt = (row: number, col: number): CellState | undefined => state.cells.get(cellKey(row, col))
  const writeCell = (row: number, col: number, cell: CellState): void => {
    state.cells.set(cellKey(row, col), cell)
  }

  const worksheet: XlsxWorksheet = {
    get name(): string {
      return state.name
    },
    toString: jest.fn((): string => `[HybridObject Worksheet ${state.name}]`),
    equals: jest.fn((other: unknown): boolean => other === worksheet),
    dispose: jest.fn((): void => undefined),

    writeString: jest.fn((row: number, col: number, value: string): void => {
      writeCell(row, col, { value, type: 'string' })
    }),
    writeNumber: jest.fn((row: number, col: number, value: number): void => {
      writeCell(row, col, { value, type: 'number' })
    }),
    writeBoolean: jest.fn((row: number, col: number, value: boolean): void => {
      writeCell(row, col, { value, type: 'boolean' })
    }),
    writeBlank: jest.fn((row: number, col: number): void => {
      writeCell(row, col, { value: null, type: 'blank' })
    }),

    writeFormula: jest.fn((row: number, col: number, formula: string): void => {
      writeCell(row, col, { value: null, type: 'formula', formula })
    }),
    writeFormulaNum: jest.fn((row: number, col: number, formula: string, number: number): void => {
      writeCell(row, col, { value: number, type: 'formula', formula })
    }),
    writeFormulaString: jest.fn((row: number, col: number, formula: string, str: string): void => {
      writeCell(row, col, { value: str, type: 'formula', formula })
    }),
    writeFormulaBoolean: jest.fn((row: number, col: number, formula: string, boolVal: boolean): void => {
      writeCell(row, col, { value: boolVal, type: 'formula', formula })
    }),
    writeArrayFormula: jest.fn((firstRow: number, firstCol: number, _lastRow: number, _lastCol: number, formula: string): void => {
      writeCell(firstRow, firstCol, { value: null, type: 'formula', formula })
    }),

    writeDatetime: jest.fn((row: number, col: number, datetime: number): void => {
      writeCell(row, col, { value: datetime, type: 'date' })
    }),
    writeURL: jest.fn((row: number, col: number, url: string): void => {
      writeCell(row, col, { value: url, type: 'string' })
    }),

    setColumn: jest.fn((): void => undefined),
    setRow: jest.fn((): void => undefined),
    deleteRow: jest.fn((): boolean => true),
    deleteColumn: jest.fn((): boolean => true),

    mergeRange: jest.fn((): void => undefined),
    mergeRangeNum: jest.fn((): void => undefined),
    unmergeCells: jest.fn((): void => undefined),

    hide: jest.fn((): void => undefined),
    activate: jest.fn((): void => undefined),

    protect: jest.fn((): void => undefined),
    protectSheet: jest.fn((): void => undefined),
    protectObjects: jest.fn((): void => undefined),
    protectScenarios: jest.fn((): void => undefined),

    setPassword: jest.fn((): void => undefined),
    setPasswordHash: jest.fn((): void => undefined),
    clearPassword: jest.fn((): void => undefined),
    clearSheetProtection: jest.fn((): void => undefined),

    allowInsertColumns: jest.fn((): void => undefined),
    allowInsertRows: jest.fn((): void => undefined),
    allowDeleteColumns: jest.fn((): void => undefined),
    allowDeleteRows: jest.fn((): void => undefined),
    allowSelectLockedCells: jest.fn((): void => undefined),
    allowSelectUnlockedCells: jest.fn((): void => undefined),

    denyInsertColumns: jest.fn((): void => undefined),
    denyInsertRows: jest.fn((): void => undefined),
    denyDeleteColumns: jest.fn((): void => undefined),
    denyDeleteRows: jest.fn((): void => undefined),
    denySelectLockedCells: jest.fn((): void => undefined),
    denySelectUnlockedCells: jest.fn((): void => undefined),

    sheetProtected: jest.fn((): boolean => false),
    objectsProtected: jest.fn((): boolean => false),
    scenariosProtected: jest.fn((): boolean => false),
    insertColumnsAllowed: jest.fn((): boolean => true),
    insertRowsAllowed: jest.fn((): boolean => true),
    deleteColumnsAllowed: jest.fn((): boolean => true),
    deleteRowsAllowed: jest.fn((): boolean => true),
    selectLockedCellsAllowed: jest.fn((): boolean => true),
    selectUnlockedCellsAllowed: jest.fn((): boolean => true),
    passwordIsSet: jest.fn((): boolean => false),
    passwordHash: jest.fn((): string => ''),
    sheetProtectionSummary: jest.fn((): string => ''),

    setColumnHidden: jest.fn((): void => undefined),
    setRowHidden: jest.fn((): void => undefined),

    setComment: jest.fn((row: number, col: number, text: string): void => {
      state.comments.set(cellKey(row, col), text)
    }),
    getComment: jest.fn((row: number, col: number): string => state.comments.get(cellKey(row, col)) ?? ''),
    hasComment: jest.fn((row: number, col: number): boolean => state.comments.has(cellKey(row, col))),
    deleteComment: jest.fn((row: number, col: number): boolean => state.comments.delete(cellKey(row, col))),
    getCommentCount: jest.fn((): number => state.comments.size),
    addCommentAuthor: jest.fn((author: string): number => {
      state.commentAuthors.push(author)
      return state.commentAuthors.length - 1
    }),
    getCommentAuthor: jest.fn((authorId: number): string => state.commentAuthors[authorId] ?? ''),

    getConditionalFormats: jest.fn((): XlsxConditionalFormats => conditionalFormats),

    getCellValue: jest.fn((row: number, col: number): string | number | boolean | null => cellAt(row, col)?.value ?? null),
    getCellString: jest.fn((row: number, col: number): string => {
      const cell = cellAt(row, col)
      return cell && cell.value !== null ? String(cell.value) : ''
    }),
    getCellRawValue: jest.fn((row: number, col: number): string => {
      const cell = cellAt(row, col)
      return cell && cell.value !== null ? String(cell.value) : ''
    }),
    getCellType: jest.fn((row: number, col: number): CellType => cellAt(row, col)?.type ?? 'empty'),
    getCellFormat: jest.fn((): XlsxCellFormat => createFakeCellFormat()),
    hasFormula: jest.fn((row: number, col: number): boolean => cellAt(row, col)?.formula != null),
    formula: jest.fn((row: number, col: number): string => cellAt(row, col)?.formula ?? ''),
    findCell: jest.fn((row: number, col: number): boolean => state.cells.has(cellKey(row, col))),

    getRowCount: jest.fn((): number => {
      const rows = new Set<number>()
      for (const key of state.cells.keys()) rows.add(Number(key.slice(0, key.indexOf(':'))))
      return rows.size
    }),
    getColumnCount: jest.fn((): number => {
      const cols = new Set<number>()
      for (const key of state.cells.keys()) cols.add(Number(key.slice(key.indexOf(':') + 1)))
      return cols.size
    }),
    getLastRow: jest.fn((): number => {
      let last = 0
      for (const key of state.cells.keys()) {
        const row = Number(key.slice(0, key.indexOf(':')))
        if (row > last) last = row
      }
      return last
    }),
    getLastColumn: jest.fn((): number => {
      let last = 0
      for (const key of state.cells.keys()) {
        const col = Number(key.slice(key.indexOf(':') + 1))
        if (col > last) last = col
      }
      return last
    }),
    getName: jest.fn((): string => state.name),

    toJSON: jest.fn((): ReturnType<XlsxWorksheet['toJSON']> => rowsFromCells(state.cells)),
    fromJSON: jest.fn((): void => undefined),
  }
  return worksheet
}

function createFakeWorkbook(): XlsxWorkbook {
  interface SheetEntry {
    state: SheetState
    worksheet: XlsxWorksheet
  }
  const sheets: SheetEntry[] = []

  const addSheet = (name: string, cells?: Map<string, CellState>): SheetEntry => {
    const state: SheetState = {
      name,
      cells: cells ?? new Map(),
      comments: new Map(),
      commentAuthors: [],
    }
    const entry: SheetEntry = { state, worksheet: createFakeWorksheet(state) }
    sheets.push(entry)
    return entry
  }
  const requireSheet = (methodName: string, name: string): SheetEntry => {
    const entry = sheets.find((sheet) => sheet.state.name === name)
    if (!entry) throw new Error(`${methodName}: SHEET_NOT_FOUND: worksheet not found: ${name}`)
    return entry
  }

  const workbook: XlsxWorkbook = {
    name: 'Workbook',
    toString: jest.fn((): string => '[HybridObject Workbook]'),
    equals: jest.fn((other: unknown): boolean => other === workbook),
    dispose: jest.fn((): void => undefined),

    addWorksheet: jest.fn((name?: string): XlsxWorksheet => addSheet(name ?? `Sheet${sheets.length + 1}`).worksheet),
    getWorksheet: jest.fn((index: number): XlsxWorksheet => {
      const entry = sheets[index]
      if (!entry) throw new Error(`getWorksheet: INDEX_OUT_OF_RANGE: no worksheet at index ${index}`)
      return entry.worksheet
    }),
    getWorksheetByName: jest.fn((name: string): XlsxWorksheet => requireSheet('getWorksheetByName', name).worksheet),
    getOrAddWorksheet: jest.fn((name: string): XlsxWorksheet => {
      const existing = sheets.find((sheet) => sheet.state.name === name)
      return (existing ?? addSheet(name)).worksheet
    }),
    getWorksheetCount: jest.fn((): number => sheets.length),
    deleteSheet: jest.fn((name: string): void => {
      const entry = requireSheet('deleteSheet', name)
      sheets.splice(sheets.indexOf(entry), 1)
    }),
    updateSheetName: jest.fn((oldName: string, newName: string): void => {
      requireSheet('updateSheetName', oldName).state.name = newName
    }),
    clone: jest.fn((existingName: string, newName: string): XlsxWorksheet =>
      addSheet(newName, new Map(requireSheet('clone', existingName).state.cells)).worksheet
    ),
    addCellFormat: jest.fn((): XlsxCellFormat => createFakeCellFormat()),
    getBuffer: jest.fn((): Promise<ArrayBuffer> => Promise.resolve(new ArrayBuffer(8))),
    toJSON: jest.fn((): ReturnType<XlsxWorkbook['toJSON']> =>
      Object.fromEntries(sheets.map((sheet) => [sheet.state.name, rowsFromCells(sheet.state.cells)]))
    ),
  }
  return workbook
}

function createFakeNitroXlsx(hybridName: string): NitroXlsx {
  const nitroXlsx: NitroXlsx = {
    name: hybridName,
    toString: jest.fn((): string => `[HybridObject ${hybridName}]`),
    equals: jest.fn((other: unknown): boolean => other === nitroXlsx),
    dispose: jest.fn((): void => undefined),

    createWorkbook: jest.fn((): XlsxWorkbook => createFakeWorkbook()),
    openWorkbook: jest.fn((): XlsxWorkbook => createFakeWorkbook()),
    openWorkbookFromBuffer: jest.fn((): XlsxWorkbook => createFakeWorkbook()),
    fromJSON: jest.fn((): XlsxWorkbook => createFakeWorkbook()),
  }
  return nitroXlsx
}

export const NitroModules = {
  createHybridObject<T>(name: string): T {
    return createFakeNitroXlsx(name) as unknown as T
  },
}
