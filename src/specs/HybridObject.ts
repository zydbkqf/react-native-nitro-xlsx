export interface HybridObject<TPlatforms extends object> {}

export type AnyHybridObject = HybridObject<any>

export type AnyMap = Record<string, any>

export type Sync<T> = T

export type CustomType<T, TName extends string, TConfig extends object = {}> = T

export type Int64 = number
export type UInt64 = number
