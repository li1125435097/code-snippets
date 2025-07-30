const grpc = require('@grpc/grpc-js');
const protoLoader = require('@grpc/proto-loader');

// 加载 protobuf
const PROTO_PATH = __dirname + '/proto/sum.proto';
const packageDefinition = protoLoader.loadSync(PROTO_PATH, {
    keepCase: true,
    longs: String,
    enums: String,
    defaults: true,
    oneofs: true
});
const sumProto = grpc.loadPackageDefinition(packageDefinition).sum;

// 创建 gRPC 客户端
const client = new sumProto.SumService(
    'localhost:50051',
    grpc.credentials.createInsecure()
);

// 加和函数
function sum(numbers) {
    return new Promise((resolve, reject) => {
        client.Sum({ numbers }, (err, response) => {
            if (err) {
                reject(err);
            } else {
                resolve(response.result);
            }
        });
    });
}

// 使用示例
async function main() {
    // 常规数字
    console.time()
    let sum1 = await sum(['37', '2', '3', '4', '5'])
    console.timeEnd()
    console.log('Sum of 1 to 5:', sum1)
    // 大整数
    const bigNumbers = [
        '123456789012345678901234567890',
        '987654321098765432109876543210'
    ];
    // console.log('Sum of big integers:', await sum(bigNumbers));
}

main().catch(console.error);