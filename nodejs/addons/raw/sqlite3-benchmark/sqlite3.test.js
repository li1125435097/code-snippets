// main.js
const sqlite3 = require('sqlite3').verbose(); // 导入 sqlite3 并启用详细日志
const fs = require('fs')
const dbpath =  './test.db'
const rows = 5000

const createTable = `CREATE TABLE IF NOT EXISTS Users(ID INTEGER PRIMARY KEY AUTOINCREMENT,Name TEXT NOT NULL,Age INTEGER);`
const querysql = `SELECT count(*) FROM Users;`
// const querysql = `SELECT * FROM Users limit 10;`
const insertsql = new Array(rows).fill(0).map((_,i)=>`INSERT INTO Users(Name, Age) VALUES('John${i}', ${i});`)
const insertsql2 ='BEGIN;'+ new Array(rows).fill(0).map((_,i)=>`INSERT INTO Users(Name, Age) VALUES('Mr${i}', ${i});`).join(';')+'COMMIT;'

async function main() {
    let xnjk = new Xnjk(1000)
    // 删除数据库文件
    if(fs.existsSync(dbpath)) fs.rmSync(dbpath, { force: true })

    // 打开数据库连接
    const db = new sqlite3.Database(dbpath)
    
    // 测试建表时间
    console.time('create table')
    await query(db,createTable)
    console.timeEnd('create table')

    // 测试插入时间
    console.time('insert')
    for (let sql of insertsql) await query(db,sql)
    console.timeEnd('insert')

    // 测试批量插入时间
    console.time('insert multi')
    await query(db,insertsql2)
    console.timeEnd('insert multi')

    // 测试查询时间
    console.time('query')
    let result = await query(db,querysql,'all')
    console.timeEnd('query')
    console.log(result)
    xnjk.stop()
}


function query(db,sql,handle='exec') {
    return new Promise((resolve, reject) => {
        db[handle](sql, (err, rows) => {
            if (err)reject(err)
            resolve(rows)
        })
    })
}

class Xnjk{
    constructor(interval=1000) {
        this.timer = this.start(interval)
    }

    stop() {
        clearInterval(this.timer)
    }

    start(interval) {
        return setInterval(() => {
            // 为了优化 Node.js 应用的性能，开发者应该尽量减少事件循环中的阻塞操作，避免在事件循环的某个阶段执行耗时较长的任务。此外，合理使用微任务和宏任务也可以帮助提高应用的响应速度
            const startTime = process.hrtime()
            setImmediate(() => { 
                const endTime = process.hrtime(startTime)
                const delayInNanoseconds = endTime[0]*1e9 + endTime[1]
                const delayInMilliseconds = delayInNanoseconds / 1e6
                console.log(`Event loop delay : ${delayInMilliseconds.toFixed(2)} ms`)
            })

            // 在Node.js中，“句柄”（handle）是一个广泛的概念，它通常指的是一个引用或指针，用于标识和访问某种资源或对象。这些资源可以是文件、网络套接字、定时器、进程等。Node.js提供了各种API来创建和管理这些句柄，使开发者能够高效地与底层系统进行交互
            const activeHandles = process._getActiveHandles() 
            let socketNum = 0
            activeHandles.forEach(v => {
                const name = v.constructor.name
                if(name === 'Socket') socketNum++
            })
            console.log(`jubingshu: ${activeHandles.length}  sockets: ${socketNum}`)

            // cpu和内存
            console.log(
                process.cpuUsage(),
                'heapTotal:' + (process.memoryUsage().heapTotal/1024/1024).toFixed(2)+'MB',
                'heapUsed:' + (process.memoryUsage().heapUsed/1024/1024).toFixed(2)+'MB'
            ) 
        },interval)
    }
}
main()
