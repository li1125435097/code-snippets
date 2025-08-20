const { readdirSync } = require('fs');

const procs = []
// 获取所有测试文件
readdirSync(__dirname).map(file => file.endsWith('.test.js') && procs.push(require(`./${file}`)))

// 同步执行所有测试文件
async function main() {
  const len = 5000000
  console.log(`洗牌算法测试开始： 洗牌数组长度为${len}`)
  for (const proc of procs) await proc(len)
}
main()
