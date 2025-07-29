/**
 * @name 问道序列化数据解析脚本。用来转换成json数据，操作后再序列化成问道数据
 * @description 使用方法：将1.wd文件放在当前目录下，运行node app.js 即可
 * @description 1.wd 文件是问道游戏中的序列化数据文件，用于存储游戏中的各种数据，解析后的数据保存在1.wd同名的文件夹中，包含1.wd文件和1.wd.json文件
 */
const {parseLPC} = require('./comm')
const {readFileSync,writeFileSync,mkdirSync,cpSync,existsSync,rmdirSync} = require('fs')
const {join} = require('path')

const fname = '1.wd'

const str = readFileSync(fname).toString()
const json = parseLPC(str)

// 将解析后的 JSON 对象保存为文件
const dirname = fname.split('.')[0]
if(existsSync(dirname)) rmdirSync(dirname,{recursive:true})
mkdirSync(dirname)
writeFileSync(join(dirname,fname),str)
writeFileSync(join(dirname,fname+'.json'),JSON.stringify(json))
cpSync(join(__dirname,'xlh.js'),join(dirname,'xlh.js'))