export type HybridObject<T> = object;

export interface ArrayBuffer {
  data(): Uint8Array;
  size(): number;
}

export interface Promise<T> {
  then<R>(onFulfilled: (value: T) => R): Promise<R>;
  catch<R>(onRejected: (reason: any) => R): Promise<R>;
  resolve(value: T): Promise<T>;
  reject(reason: any): Promise<T>;
  create(): Promise<T>;
}

export namespace margelo {
  export namespace nitro {
    export class HybridObject<T> {}
  }
}

export const NitroModules = {
  createHybridObject: <T>(name: string) => ({
    createWorkbook: jest.fn(),
    openWorkbook: jest.fn(),
    openWorkbookFromBuffer: jest.fn(),
  }),
};