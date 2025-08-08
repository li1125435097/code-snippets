const { runInCallbackScope,testResolveAsync } = require(`./build/Release/binding`);
const l = console.log;
const util = require('util');
const async_hooks = require('async_hooks');

// 查看调用过程
async_hooks.createHook({
  init(asyncId, type, triggerAsyncId, resource) {
    process._rawDebug(util.format('Init: %s(%d)', type, asyncId));
  },
  before(asyncId) {
    process._rawDebug(`Before: ${asyncId}`);
  },
  after(asyncId) {
    process._rawDebug(`After: ${asyncId}`);
  },
  destroy(asyncId) {
    process._rawDebug(`Destroy: ${asyncId}`);
  },
  promiseResolve(asyncId) {
    process._rawDebug(`PromiseResolve: ${asyncId}`);
  }
}).enable();


let a = runInCallbackScope({a:123}, 1020, 1001, function(a,b,c) {
  l(a);
  return 12344;
});
l(a,333)

l('--------------------------------------------------')


;(async () => {
  let dd = await testResolveAsync(666, function(a,b,c) {
    l(333)
  })
  l(dd, 444)
})()

