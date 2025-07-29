const {stringifyLPC} = require('../comm')
const {writeFileSync,readdirSync} = require('fs')
const {join} = require('path')

const jsonf = readdirSync(__dirname).find(x=>x.endsWith('.json'))
const jsonData = require(join(__dirname,jsonf))

writeFileSync(jsonf+'.out',stringifyLPC(jsonData))