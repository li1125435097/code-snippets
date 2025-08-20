const { readdirSync, existsSync } = require('fs');

const dirs = []
// 获取所有目录
readdirSync(__dirname).map(file => !file.endsWith('.js') && dirs.push(file))

// 同步执行所有目录下的test.js文件
const procs = []
dirs.map(dir => existsSync(`${__dirname}/${dir}/test.js`) && procs.push(require(`./${dir}/test`)))