import {
  NitroXlsx,
  Align,
  VAlign,
  Border,
  Underline,
  Script,
  Pattern,
  Paper,
  Gridlines,
  Colors,
  NumFormat,
  XlsxError,
  XlsxErrorCode,
  CfType,
  CfOperator,
  CfTimePeriod,
} from '../index';

describe('NitroXlsx', () => {
  beforeEach(() => {
    jest.clearAllMocks();
  });

  it('should export NitroXlsx object', () => {
    expect(NitroXlsx).toBeDefined();
    expect(typeof NitroXlsx).toBe('object');
  });

  it('createWorkbook returns a workbook implementing the spec surface', () => {
    const wb = NitroXlsx.createWorkbook();
    expect(typeof wb.addWorksheet).toBe('function');
    expect(typeof wb.getBuffer).toBe('function');
    expect(typeof wb.toJSON).toBe('function');
    expect(wb.getWorksheetCount()).toBe(0);
  });

  it('addWorksheet registers the sheet and forwards its argument', () => {
    const wb = NitroXlsx.createWorkbook();
    const ws = wb.addWorksheet('Data');
    expect(ws.getName()).toBe('Data');
    expect(wb.getWorksheetCount()).toBe(1);
    expect(wb.getWorksheet(0)).toBe(ws);
    expect(jest.mocked(wb.addWorksheet).mock.calls[0]).toEqual(['Data']);
  });

  it('worksheet write/read methods round-trip cell values', () => {
    const ws = NitroXlsx.createWorkbook().addWorksheet('Data');
    ws.writeNumber(1, 2, 3.5);
    expect(ws.getCellValue(1, 2)).toBe(3.5);
    expect(ws.getCellType(1, 2)).toBe('number');
    ws.writeString(0, 0, 'Hello');
    expect(ws.getCellString(0, 0)).toBe('Hello');
    ws.writeFormula(3, 3, 'A1+B1');
    expect(ws.hasFormula(3, 3)).toBe(true);
    expect(ws.formula(3, 3)).toBe('A1+B1');
    expect(ws.getCellValue(9, 9)).toBeNull();
  });

  it('getBuffer resolves an ArrayBuffer', async () => {
    const wb = NitroXlsx.createWorkbook();
    await expect(wb.getBuffer()).resolves.toBeInstanceOf(ArrayBuffer);
  });

  it('toJSON returns rows keyed by sheet name', () => {
    const wb = NitroXlsx.createWorkbook();
    const ws = wb.addWorksheet('Data');
    ws.writeString(0, 0, 'Hello');
    ws.writeNumber(0, 1, 42);
    expect(wb.toJSON()).toEqual({ Data: [{ '0': 'Hello', '1': 42 }] });
  });

  it('openWorkbook forwards the path and returns a workbook', () => {
    const wb = NitroXlsx.openWorkbook('/tmp/report.xlsx');
    expect(jest.mocked(NitroXlsx.openWorkbook).mock.calls[0]).toEqual(['/tmp/report.xlsx']);
    expect(typeof wb.addWorksheet).toBe('function');
    expect(wb.getWorksheetCount()).toBe(0);
  });

  it('openWorkbookFromBuffer forwards the buffer', () => {
    const buffer = new ArrayBuffer(16);
    NitroXlsx.openWorkbookFromBuffer(buffer);
    expect(jest.mocked(NitroXlsx.openWorkbookFromBuffer).mock.calls[0]?.[0]).toBe(buffer);
  });

  it('fromJSON forwards the sheet rows and returns a workbook', () => {
    const data = { Sheet1: [{ Name: 'Alice', Age: 30 }] };
    const wb = NitroXlsx.fromJSON(data);
    expect(jest.mocked(NitroXlsx.fromJSON).mock.calls[0]).toEqual([data]);
    expect(typeof wb.addWorksheet).toBe('function');
  });

  it('sheet lookup, rename, clone and delete follow the native contract', () => {
    const wb = NitroXlsx.createWorkbook();
    const ws = wb.addWorksheet('Data');
    wb.updateSheetName('Data', 'Raw');
    expect(ws.getName()).toBe('Raw');
    const copy = wb.clone('Raw', 'Copy');
    expect(copy.getName()).toBe('Copy');
    expect(wb.getWorksheetCount()).toBe(2);
    wb.deleteSheet('Copy');
    expect(wb.getWorksheetCount()).toBe(1);
  });

  it('lookup misses throw native-style errors parseable by XlsxError.from', () => {
    expect.assertions(2);
    const wb = NitroXlsx.createWorkbook();
    try {
      wb.getWorksheetByName('missing');
    } catch (e) {
      const err = XlsxError.from(e);
      expect(err.code).toBe('SHEET_NOT_FOUND');
      expect(err.message).toContain('missing');
    }
  });

  it('cell format setters update the read-back getters', () => {
    const fmt = NitroXlsx.createWorkbook().addCellFormat();
    fmt.setBold();
    expect(fmt.getIsBold()).toBe(true);
    fmt.setNumFormat('0.00%');
    expect(fmt.getNumFormat()).toBe('0.00%');
    fmt.setFontSize(14);
    expect(fmt.getFontSize()).toBe(14);
  });

  it('conditional formats create rules and round-trip rule edits', () => {
    const ws = NitroXlsx.createWorkbook().addWorksheet('Data');
    const cfs = ws.getConditionalFormats();
    const cf = cfs.create('A1:A5');
    expect(cf.getSqref()).toBe('A1:A5');
    expect(cfs.getCount()).toBe(1);
    const rule = cf.createRule();
    expect(rule).toBe(0);
    cf.setRuleType(rule, 1);
    cf.setRuleFormula(rule, 0, 'A1>5');
    expect(cf.getRuleType(rule)).toBe(1);
    expect(cf.getRuleFormula(rule, 0)).toBe('A1>5');
  });
});

describe('Constants', () => {
  describe('Align', () => {
    it('should export Align constants', () => {
      expect(Align).toBeDefined();
      expect(Align.GENERAL).toBe(0);
      expect(Align.LEFT).toBe(1);
      expect(Align.RIGHT).toBe(2);
      expect(Align.CENTER).toBe(3);
    });
  });

  describe('VAlign', () => {
    it('should export VAlign constants', () => {
      expect(VAlign).toBeDefined();
      expect(VAlign.TOP).toBe(4);
      expect(VAlign.CENTER).toBe(3);
      expect(VAlign.BOTTOM).toBe(5);
    });
  });

  describe('Border', () => {
    it('should export Border constants', () => {
      expect(Border).toBeDefined();
      expect(Border.NONE).toBe(0);
      expect(Border.THIN).toBe(1);
      expect(Border.MEDIUM).toBe(2);
      expect(Border.THICK).toBe(5);
    });
  });

  describe('Underline', () => {
    it('should export Underline constants', () => {
      expect(Underline).toBeDefined();
      expect(Underline.NONE).toBe(0);
      expect(Underline.SINGLE).toBe(1);
      expect(Underline.DOUBLE).toBe(2);
    });
  });

  describe('Script', () => {
    it('should export Script constants', () => {
      expect(Script).toBeDefined();
      expect(Script.BASELINE).toBe(0);
      expect(Script.SUBSCRIPT).toBe(1);
      expect(Script.SUPERSCRIPT).toBe(2);
    });
  });

  describe('Pattern', () => {
    it('should export Pattern constants', () => {
      expect(Pattern).toBeDefined();
      expect(Pattern.NONE).toBe(0);
      expect(Pattern.SOLID).toBe(1);
    });
  });

  describe('Paper', () => {
    it('should export Paper constants', () => {
      expect(Paper).toBeDefined();
      expect(Paper.LETTER).toBe(1);
      expect(Paper.A4).toBe(9);
    });
  });

  describe('Gridlines', () => {
    it('should export Gridlines constants', () => {
      expect(Gridlines).toBeDefined();
      expect(Gridlines.HIDE_ALL).toBe(0);
      expect(Gridlines.SHOW_ALL).toBe(1);
    });
  });

  describe('Colors', () => {
    it('should export Colors constants', () => {
      expect(Colors).toBeDefined();
      expect(Colors.BLACK).toBe(0x000000);
      expect(Colors.WHITE).toBe(0xFFFFFF);
      expect(Colors.RED).toBe(0xFF0000);
      expect(Colors.GREEN).toBe(0x00FF00);
      expect(Colors.BLUE).toBe(0x0000FF);
    });
  });

  describe('NumFormat', () => {
    it('should export NumFormat constants', () => {
      expect(NumFormat).toBeDefined();
      expect(NumFormat.GENERAL).toBe(0);
      expect(NumFormat.NUMBER).toBe(1);
      expect(NumFormat.CURRENCY).toBe(5);
      expect(NumFormat.PERCENT).toBe(9);
      expect(NumFormat.SCIENTIFIC).toBe(11);
      expect(NumFormat.FRACTION_1).toBe(12);
      expect(NumFormat.DATE_1).toBe(14);
      expect(NumFormat.TIME_1).toBe(18);
      expect(NumFormat.DATETIME).toBe(22);
      expect(NumFormat.TEXT).toBe(49);
    });

    it('should not expose nonexistent built-in numFmtIds', () => {
      expect('SCIENTIFIC_2' in NumFormat).toBe(false);
      expect('DATE_5' in NumFormat).toBe(false);
    });
  });

  describe('CfType', () => {
    it('should export CfType constants', () => {
      expect(CfType).toBeDefined();
      expect(CfType.EXPRESSION).toBe(0);
      expect(CfType.CELL_IS).toBe(1);
      expect(CfType.TOP10).toBe(5);
      expect(CfType.CONTAINS_TEXT).toBe(8);
      expect(CfType.TIME_PERIOD).toBe(16);
    });
  });

  describe('CfOperator', () => {
    it('should export CfOperator constants', () => {
      expect(CfOperator).toBeDefined();
      expect(CfOperator.LESS_THAN).toBe(0);
      expect(CfOperator.EQUAL).toBe(2);
      expect(CfOperator.GREATER_THAN).toBe(5);
      expect(CfOperator.BETWEEN).toBe(6);
    });
  });

  describe('CfTimePeriod', () => {
    it('should export CfTimePeriod constants', () => {
      expect(CfTimePeriod).toBeDefined();
      expect(CfTimePeriod.TODAY).toBe(0);
      expect(CfTimePeriod.YESTERDAY).toBe(1);
      expect(CfTimePeriod.THIS_WEEK).toBe(7);
    });
  });
});

describe('XlsxError', () => {
  it('should export XlsxErrorCode constants', () => {
    expect(XlsxErrorCode.INVALID_ARGUMENT).toBe('INVALID_ARGUMENT');
    expect(XlsxErrorCode.SHEET_NOT_FOUND).toBe('SHEET_NOT_FOUND');
    expect(XlsxErrorCode.INDEX_OUT_OF_RANGE).toBe('INDEX_OUT_OF_RANGE');
    expect(XlsxErrorCode.UNSUPPORTED).toBe('UNSUPPORTED');
    expect(XlsxErrorCode.XLSX_ERROR).toBe('XLSX_ERROR');
  });

  it('should create XlsxError with code and message', () => {
    const err = new XlsxError(XlsxErrorCode.SHEET_NOT_FOUND, 'Sheet missing');
    expect(err).toBeInstanceOf(Error);
    expect(err.name).toBe('XlsxError');
    expect(err.code).toBe('SHEET_NOT_FOUND');
    expect(err.message).toContain('SHEET_NOT_FOUND');
    expect(err.message).toContain('Sheet missing');
  });

  it('should parse native error message into XlsxError', () => {
    const native = new Error('deleteSheet: SHEET_NOT_FOUND: Worksheet not found: Foo');
    const err = XlsxError.from(native);
    expect(err.code).toBe('SHEET_NOT_FOUND');
    expect(err.message).toContain('Worksheet not found: Foo');
  });

  it('should parse native INVALID_ARGUMENT errors', () => {
    const native = new Error('writeNumber: INVALID_ARGUMENT: bad arg');
    const err = XlsxError.from(native);
    expect(err.code).toBe('INVALID_ARGUMENT');
    expect(err.message).toContain('bad arg');
  });

  it('should wrap unknown errors with INTERNAL_ERROR', () => {
    const err = XlsxError.from(new Error('something odd'));
    expect(err.code).toBe('INTERNAL_ERROR');
  });

  it('should return the same instance if already XlsxError', () => {
    const original = new XlsxError(XlsxErrorCode.IO_ERROR, 'disk');
    expect(XlsxError.from(original)).toBe(original);
  });
});