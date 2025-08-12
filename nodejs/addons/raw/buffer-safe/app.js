const addon = require('./build/Release/addon')
const l = console.log
l(addon)
// Flags: --expose-gc
addon.run();
global.gc();
l(addon.isAlive())
setImmediate(() => l(addon.isAlive()));