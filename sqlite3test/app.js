const sqlite3 = require('sqlite3-pure');

sqlite3.openDb('test.db'); // Use in-memory database
sqlite3.exec('CREATE TABLE if not exists test (id INTEGER, name TEXT); INSERT INTO test VALUES (1, "Alice"), (2, "Bob"); SELECT * FROM test;');

let a = sqlite3.exec('SELECT * FROM test')
console.log(a);