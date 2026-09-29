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
  it('should export NitroXlsx object', () => {
    expect(NitroXlsx).toBeDefined();
    expect(typeof NitroXlsx).toBe('object');
  });

  it('should have createWorkbook method', () => {
    expect(NitroXlsx.createWorkbook).toBeDefined();
    expect(typeof NitroXlsx.createWorkbook).toBe('function');
  });

  it('should have openWorkbook method', () => {
    expect(NitroXlsx.openWorkbook).toBeDefined();
    expect(typeof NitroXlsx.openWorkbook).toBe('function');
  });

  it('should have openWorkbookFromBuffer method', () => {
    expect(NitroXlsx.openWorkbookFromBuffer).toBeDefined();
    expect(typeof NitroXlsx.openWorkbookFromBuffer).toBe('function');
  });

  it('should have fromJSON method that accepts a Record of sheet rows', () => {
    expect(NitroXlsx.fromJSON).toBeDefined();
    expect(typeof NitroXlsx.fromJSON).toBe('function');
    expect(() =>
      NitroXlsx.fromJSON({ Sheet1: [{ Name: 'Alice', Age: 30 }] })
    ).not.toThrow();
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
      expect(NumFormat.DATE_1).toBe(15);
      expect(NumFormat.TEXT).toBe(49);
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

  it('should wrap unknown errors with INTERNAL_ERROR', () => {
    const err = XlsxError.from(new Error('something odd'));
    expect(err.code).toBe('INTERNAL_ERROR');
  });

  it('should return the same instance if already XlsxError', () => {
    const original = new XlsxError(XlsxErrorCode.IO_ERROR, 'disk');
    expect(XlsxError.from(original)).toBe(original);
  });
});