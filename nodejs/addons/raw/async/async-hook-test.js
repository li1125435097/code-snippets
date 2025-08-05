const { runInCallbackScope,testResolveAsync } = require(`./build/Release/binding`);
const l = console.log;

let a = runInCallbackScope({a:123}, 1000, 1000, (a,b,c) => {
  l(a,b,c,123);
});
l(a,333)