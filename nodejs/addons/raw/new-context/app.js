const {  makeBufferInNewContext,runInNewContext} = require('./build/Release/addon')

// Because the `Buffer` function and its prototype property only (currently)
// exist in a Node.js instance’s main context, trying to create buffers from
// another context throws an exception.
try {
  makeBufferInNewContext()
} catch (error) {
  console.log(error.message);
}

// 当前context会输出到当前控制台，而新context在当前控制台不输出，data是返回值，可以输出到当前控制台
let data = runInNewContext("console.log(1);data",{ext:{a:12}});
console.log(data);



