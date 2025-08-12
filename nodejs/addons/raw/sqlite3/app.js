const addon = require('./build/Release/addon')
const fs = require('fs')
const l = console.log

// const version = ['sync','uv_cb','uv_promise'][0]
let version = fs.readFileSync('./binding.gyp','utf-8').trim()
version = version.match(/"sources": \[ ".+\.cc" \]/)[0].slice(14,-6)
addon.OpenDb('./test.db')

if(version === 'addon'){
    addon.Exec(`CREATE TABLE IF NOT EXISTS Users(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        Name TEXT NOT NULL,
        Age INTEGER);`)

    addon.Exec(`INSERT INTO Users(Name, Age) VALUES('John', 30);`)
    addon.Exec(`INSERT INTO Users(Name, Age) VALUES('Jane', 25);`)
    addon.Exec(`INSERT INTO Users(Name, Age) VALUES('Bob', 40);`)

    l(addon.Exec(`SELECT * FROM Users;`))
}else if(version === 'uv-cb'){
    addon.Exec(`SELECT * FROM Users;`, (a,b) => {
        l(b)
    })
    
}else if(version === 'uv-promise'){
    addon.Exec(`SELECT * FROM Users;`).then((res) => {
        l(res)
    })   
}

