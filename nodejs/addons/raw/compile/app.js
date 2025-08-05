const addon = require('./build/Release/addon');

try {
  const result = addon.executeScript("1 + 2");
  console.log(`Result:  ${result}`);  // Output: Result: 3

  const result2 = addon.executeScript("'Hello, World!'");
  console.log(`Result:  ${result2}`);  // Output: Result: Hello, World!

  const result3 = addon.executeScript("a={ name: 'Alice', age: 30 };b=50;console.log(b);a.age = b;console.log(a)");
  console.log(`Result:`, result3);  // Output: Result: { name: 'Alice', age: 30 }
} catch (error) {
  console.error(error.message);
}