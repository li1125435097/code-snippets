const {leakHandle:settimeout1} = require('./build/Release/addon')
const l = console.log

// 自己实现一个settimeout
settimeout1(1,() => l('leaked handle 1'));
settimeout1(2,() => l('leaked handle 2'));
settimeout1(3,() => l('leaked handle 3'));

