// 汉字机存16进制数据解析其编码脚本，主要用来分析乱码数据所属编码
function hexDecode(hexStr) {
    const bytes = hexStr.match(/.{2}/g).map(b => parseInt(b, 16));
    // 尝试常见编码
    const encodings = ['utf-8', 'gbk', 'big5', 'shift-jis', 'euc-kr'];
    console.log('元数据：', hexStr)
    encodings.map(v => console.log(v,': ',new TextDecoder(v).decode(new Uint8Array(bytes))))
}

let hexStr = 'C1FAD0A5BEC5CCEC';
hexDecode(hexStr);

// 输出
// 元数据： C1FAD0A5BEC5CCEC
// utf-8 :  ��Х����
// gbk :  龙啸九天
// big5 :  韓苭嬝毞
// shift-jis :  ﾁ昻･ｾﾅﾌ�
// euc-kr :  질奎씽莖
