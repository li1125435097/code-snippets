const addon = require('./build/Release/addon');

const result = addon.processTypes(
  undefined,
  null,
  true,
  42,
  3.14,
  "hello",
  { a: 1 },
  [1, 2, 3],
  () => {},
  9007199254740991n,
  Symbol("sym"),
  new Date(),
  /abc/i,
  new ArrayBuffer(16),
  new Uint8Array(4),
  new DataView(new ArrayBuffer(8)),
  new Map([['k', 'v']]),
  new Set([1, 2, 3, 'a', 'b']),
);

console.log(result);