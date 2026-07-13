/** @type {import('ts-jest').JestConfigWithTsJest} */
export default {
  preset: 'ts-jest/presets/default-esm',
  testEnvironment: 'node',
  roots: ['<rootDir>/src'],
  testMatch: ['**/__tests__/**/*.test.ts'],
  moduleNameMapper: {
    'react-native': '<rootDir>/src/__mocks__/react-native.ts',
    'react-native-nitro-modules': '<rootDir>/src/__mocks__/react-native-nitro-modules.ts',
  },
  extensionsToTreatAsEsm: ['.ts'],
  watchman: false,
  globals: {
    'ts-jest': {
      useESM: true,
      isolatedModules: true,
    },
  },
};