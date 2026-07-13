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
  ImagePosition,
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

  describe('ImagePosition', () => {
    it('should export ImagePosition constants', () => {
      expect(ImagePosition).toBeDefined();
      expect(ImagePosition.MOVE_AND_SIZE).toBe(1);
      expect(ImagePosition.MOVE_DONT_SIZE).toBe(2);
      expect(ImagePosition.DONT_MOVE_DONT_SIZE).toBe(3);
    });
  });
});