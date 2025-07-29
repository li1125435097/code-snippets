const { inspect } = require('util');

// 解析 LPC 字符串为 JavaScript 对象
function parseLPC(lpcStr) {
  let index = 0;
  const len = lpcStr.length;

  // 跳过空白字符
  function skipWhitespace() {
    while (index < len && /\s/.test(lpcStr[index])) index++;
  }

  // 解析字符串
  function parseString() {
    if (lpcStr[index] !== '"') return null;
    let result = '';
    index++; // 跳过开头的 "
    
    while (index < len && lpcStr[index] !== '"') {
      if (lpcStr[index] === '\\') {
        index++;
        switch (lpcStr[index]) {
          case 'n': result += '\n'; break;
          case 'r': result += '\r'; break;
          case 't': result += '\t'; break;
          default: result += lpcStr[index];
        }
      } else {
        result += lpcStr[index];
      }
      index++;
    }
    
    if (lpcStr[index] === '"') index++;
    return result;
  }

  // 解析数字
  function parseNumber() {
    const start = index;
    // 处理负号
    if (lpcStr[index] === '-') index++;
    
    // 整数部分
    while (index < len && /\d/.test(lpcStr[index])) index++;
    
    // 小数部分
    if (lpcStr[index] === '.') {
      index++;
      while (index < len && /\d/.test(lpcStr[index])) index++;
    }
    
    // 科学计数法
    if (/[eE]/.test(lpcStr[index])) {
      index++;
      if (/[+-]/.test(lpcStr[index])) index++;
      while (index < len && /\d/.test(lpcStr[index])) index++;
    }
    
    return parseFloat(lpcStr.slice(start, index));
  }

  // 解析值
  function parseValue() {
    skipWhitespace();
    
    // 解析映射 ([...])
    if (lpcStr.startsWith('([', index)) {
      index += 2;
      return parseMapping();
    }
    
    // 解析数组 ({...})
    if (lpcStr.startsWith('({', index)) {
      index += 2;
      return parseArray();
    }
    
    // 解析字符串
    if (lpcStr[index] === '"') {
      return parseString();
    }
    
    // 解析数字（包括负数）
    if (/-?\d/.test(lpcStr[index]+lpcStr[index+1])) {
      return parseNumber();
    }
    
    // 解析特殊值
    if (lpcStr.startsWith('0', index)) {
      index++;
      return 0;
    }
    
    throw new Error(`Unexpected token at position ${index} '${lpcStr[index]}'`);
  }

  // 解析映射
  function parseMapping() {
    const map = {};
    skipWhitespace();
    
    while (index < len && !lpcStr.startsWith('])', index)) {
      // 解析键（数字或字符串）
      let key;
      skipWhitespace();
      
      if (lpcStr[index] === '"') {
        key = parseString();
      } else {
        key = parseNumber();
      }
      
      skipWhitespace();
      
      // 跳过冒号
      if (lpcStr[index] === ':') {
        index++;
      } else {
        throw new Error(`Expected colon at position ${index}`);
      }
      
      // 解析值
      const value = parseValue();
      map[key] = value;
      
      skipWhitespace();
      
      // 跳过逗号
      if (lpcStr[index] === ',') {
        index++;
      }
      
      skipWhitespace();
    }
    
    if (lpcStr.startsWith('])', index)) {
      index += 2;
    } else {
      throw new Error(`Expected closing ]) at position ${index}`);
    }
    
    return map;
  }

  // 解析数组
  function parseArray() {
    const arr = [];
    skipWhitespace();
    
    while (index < len && !lpcStr.startsWith('})', index)) {
      arr.push(parseValue());
      skipWhitespace();
      
      // 跳过逗号
      if (lpcStr[index] === ',') {
        index++;
        skipWhitespace();
      }
    }
    
    if (lpcStr.startsWith('})', index)) {
      index += 2;
    } else {
      throw new Error(`Expected closing }) at position ${index}`);
    }
    
    return arr;
  }

  return parseValue();
}

// 将 JavaScript 对象转换为 LPC 字符串
function stringifyLPC(obj,slefkey) {
  // 处理数组
  if (Array.isArray(obj)) {
    const items = obj.map(item => stringifyLPC(item));
    return `({${items.join(',')}${items.length > 0 ? ',' : ''}})`; // 注意结尾的括号组合
  }
  
  // 处理对象
  if (obj && typeof obj === 'object') {
    const entries = Object.entries(obj);
    const items = entries.map(([key, value]) => {
      // 数字键不加引号，字符串键加双引号
      let formattedKey = /^\d+$/.test(key) ? key : `"${key}"`;
      // key.match('finish') && console.log(['finish'].includes(key),value)
      
      // 字符串键加双引号的父键处理
      if(['finish','limit_per_day','limit_per_week',"td","task_round","online_time","limit_per_month","scroll","appellation_tasks","task_score","discover_world"].includes(slefkey)) formattedKey = `"${key}"`;
      return `${formattedKey}:${stringifyLPC(value,key)}`;
    });
    return `([${items.join(',')}${items.length > 0 ? ',' : ''}])`; // 注意结尾的括号组合
  }
  
  // 处理字符串（添加转义）
  if (typeof obj === 'string') {
    const escaped = obj
      .replace(/\\/g, '\\\\')
      .replace(/"/g, '\\"')
      // .replace(/\n/g, '\\n')
      // .replace(/\r/g, '\\r')
      .replace(/\t/g, '\\t');
    return `"${escaped}"`;
  }
  
  // 处理数字和空值
  return obj === null ? '0' : String(obj);
}

// 示例使用
const lpcData = `
([
    "melt_pet":([
        "materials":([
            116: 1,
            115: 3,
            120: 2,
            118: 2,
        ]),
    ]),
    "position":([
        "room": "/tianyongcheng/tianyongcheng.c",
        "dir": 5,
        "x": 326,
        "y": 255,
    ]),
    "me":([
        "resist_lost": 0,
        "durability": 100,
        "life": 801079,
        "religion": 1,
        "type": 1,
        "stat_coin_cost":([
            "wcoin": 164760,
            "mcoin": 164760,
            "winfo":({}),
            "minfo":({}),
            "wuptime": 1752422400,
            "muptime": 1751299200,
            "lstime": 1752888450,
        ]),
        "friend_converted": 3,
        "earth": 0,
        "rec_att":({
            0,
            4,
            0,
            0,
            1,
        }),
    ]),
])
`;

// LPC 转 JSON
// const jsonData = parseLPC(lpcData);
// console.log("JSON 格式:");
// console.log(JSON.stringify(jsonData, null, 2));

// // JSON 转回 LPC
// const convertedLpc = stringifyLPC(jsonData);
// console.log("\nLPC 格式:");
// console.log(convertedLpc);

// // 验证转换正确性（功能测试）
// console.log("\n转换一致性验证:", 
//   parseLPC(convertedLpc).me.rec_att.join(',') === "0,4,0,0,1"
// );

module.exports = {
  parseLPC,
  stringifyLPC,
}