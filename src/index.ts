import { NitroModules } from 'react-native-nitro-modules'
import type { NitroXlsx as NitroXlsxSpec } from './specs/NitroXlsx.nitro'

export const NitroXlsx = NitroModules.createHybridObject<NitroXlsxSpec>('NitroXlsx')

export type { XlsxWorkbook, XlsxWorksheet, XlsxFormat } from './specs/NitroXlsx.nitro'

// Cell alignment
export const Align = {
  NONE: 0,
  LEFT: 1,
  CENTER: 2,
  RIGHT: 3,
  FILL: 4,
  JUSTIFY: 5,
  CENTER_ACROSS: 6,
  DISTRIBUTED: 7,
} as const

export const VAlign = {
  NONE: 0,
  TOP: 1,
  CENTER: 2,
  BOTTOM: 3,
  JUSTIFY: 4,
  DISTRIBUTED: 5,
} as const

// Border styles
export const Border = {
  NONE: 0,
  THIN: 1,
  MEDIUM: 2,
  DASHED: 3,
  DOTTED: 4,
  THICK: 5,
  DOUBLE: 6,
  HAIR: 7,
  MEDIUM_DASHED: 8,
  DASH_DOT: 9,
  MEDIUM_DASH_DOT: 10,
  DASH_DOT_DOT: 11,
  MEDIUM_DASH_DOT_DOT: 12,
  SLANT_DASH_DOT: 13,
} as const

// Underline styles
export const Underline = {
  NONE: 0,
  SINGLE: 1,
  DOUBLE: 2,
  SINGLE_ACCOUNTING: 0x21,
  DOUBLE_ACCOUNTING: 0x22,
} as const

// Script styles (superscript/subscript)
export const Script = {
  NONE: 0,
  SUPERSCRIPT: 1,
  SUBSCRIPT: 2,
} as const

// Pattern fill styles
export const Pattern = {
  NONE: 0,
  SOLID: 1,
  MEDIUM_GRAY: 2,
  DARK_GRAY: 3,
  LIGHT_GRAY: 4,
  DARK_HORIZONTAL: 5,
  DARK_VERTICAL: 6,
  DARK_DOWN: 7,
  DARK_UP: 8,
  DARK_GRID: 9,
  DARK_TRELLIS: 10,
  LIGHT_HORIZONTAL: 11,
  LIGHT_VERTICAL: 12,
  LIGHT_DOWN: 13,
  LIGHT_UP: 14,
  LIGHT_GRID: 15,
  LIGHT_TRELLIS: 16,
  GRAY_125: 17,
  GRAY_0625: 18,
} as const

// Paper types
export const Paper = {
  DEFAULT: 0,
  LETTER: 1,
  LETTER_SMALL: 2,
  TABLOID: 3,
  LEDGER: 4,
  LEGAL: 5,
  STATEMENT: 6,
  EXECUTIVE: 7,
  A3: 8,
  A4: 9,
  A4_SMALL: 10,
  A5: 11,
  B4: 12,
  B5: 13,
  FOLIO: 14,
  QUARTO: 15,
  SIZE10x14: 16,
  SIZE11x17: 17,
  NOTE: 18,
  ENV_9: 19,
  ENV_10: 20,
  ENV_11: 21,
  ENV_12: 22,
  ENV_14: 23,
  ENV_DL: 27,
  ENV_C5: 28,
  ENV_C3: 29,
  ENV_C4: 30,
  ENV_C6: 31,
  ENV_C65: 32,
  ENV_B4: 33,
  ENV_B5: 34,
  ENV_B6: 35,
  ENV_ITALY: 36,
  ENV_MONARCH: 37,
  ENV_PERSONAL: 38,
  ENV_US9: 39,
  ENV_US10: 40,
  ENV_US11: 41,
  ENV_US12: 42,
} as const

// Gridline options
export const Gridlines = {
  HIDE_ALL: 0,
  SHOW_ALL: 1,
  SHOW_PRINT: 2,
} as const

// Common colors
export const Colors = {
  BLACK: 0x000000,
  WHITE: 0xFFFFFF,
  RED: 0xFF0000,
  GREEN: 0x00FF00,
  BLUE: 0x0000FF,
  YELLOW: 0xFFFF00,
  CYAN: 0x00FFFF,
  MAGENTA: 0xFF00FF,
  GRAY: 0x808080,
  GREY: 0x808080,
  ORANGE: 0xFFA500,
  PURPLE: 0x800080,
  PINK: 0xFFC0CB,
  BROWN: 0xA52A2A,
  LIME: 0x00FF00,
  NAVY: 0x000080,
  TEAL: 0x008080,
  MAROON: 0x800000,
  OLIVE: 0x808000,
  SILVER: 0xC0C0C0,
  AQUA: 0x00FFFF,
  FUCHSIA: 0xFF00FF,
} as const

// Built-in number format indices
export const NumFormat = {
  GENERAL: 0,
  NUMBER: 1,
  NUMBER_2: 2,
  NUMBER_SEP: 3,
  NUMBER_SEP_2: 4,
  CURRENCY: 5,
  CURRENCY_2: 6,
  CURRENCY_SYMBOL: 7,
  CURRENCY_SYMBOL_2: 8,
  PERCENT: 9,
  PERCENT_2: 10,
  SCIENTIFIC: 11,
  SCIENTIFIC_2: 12,
  FRACTION_1: 13,
  FRACTION_2: 14,
  DATE_1: 15,
  DATE_2: 16,
  DATE_3: 17,
  DATE_4: 18,
  DATE_5: 19,
  TIME_1: 20,
  TIME_2: 21,
  TIME_3: 22,
  TIME_4: 23,
  DATETIME: 22,
  TEXT: 49,
} as const

// Image object position
export const ImagePosition = {
  MOVE_AND_SIZE: 1,
  MOVE_DONT_SIZE: 2,
  DONT_MOVE_DONT_SIZE: 3,
} as const