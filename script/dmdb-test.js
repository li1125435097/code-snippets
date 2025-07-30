const dmdb = require('dmdb')
const param = "dm://dbname:password@ip:5236?loginEncrypt=false"

// 建表
// CREATE TABLE "dev"."bmtest"
// (
// "id" INT IDENTITY(1, 1) NOT NULL,
// "ct" TIMESTAMP(6) DEFAULT CURRENT_TIMESTAMP NOT NULL,
// "ut" TIMESTAMP(6) DEFAULT CURRENT_TIMESTAMP NOT NULL,
// "a" VARCHAR2(255),
// "b" VARCHAR2(255),
// "c" NUMBER,
// "d" NUMBER,
// "e" VARCHAR2(255),
// "f" VARCHAR2(255),
// "g" DATETIME(6),
// CONSTRAINT "PK_ec4a142af397933fadd0ba5d14c" NOT CLUSTER PRIMARY KEY("id")) STORAGE(ON "YISHI", CLUSTERBTR) ;
// CREATE OR REPLACE  INDEX "IDX_d7a71c1b33708128afa7acf0fb" ON "YISHI"."bmtest"("ct" ASC) STORAGE(ON "YISHI", CLUSTERBTR) ;
// CREATE OR REPLACE  INDEX "IDX_e819023933df4249a65fedd82c" ON "YISHI"."bmtest"("ut" ASC) STORAGE(ON "YISHI", CLUSTERBTR) ;

/**
 * 达梦数据库插入性能测试语句
 * 测试时需配置count和qps两个参数
 * 运行时需设置临时依赖路径，可以是system_server的node_modules  `export NODE_PATH='/home/yishi/codePlace/208_command_server/node_modules' && node test.js`
 */
async function main(){
    const connection = await dmdb.getConnection(param).catch(err => {
      console.log('Dadb 连接错误： ',err, param)
    })

	//let sessions = await connection.execute(` select * from yishi."sub_system" where "id"=24`)
	const str = ['liangjiang','哦第四耐，法努力科技','sunchengbin','纯与的老鈤','forovertheworld','按照 中队>低慢小>独立摄像头 排序','emeisha','重庆老何','奥巴马吃桑巴']
	let index = 0,num = 0
	const count = 500
	const qps = 1 
	console.time()
	for(let k of new Array(count).fill(0)){
	//connection.execute(`select * from yishi."sub_system" where "id"=24`)
	index = ++num%10
	connection.execute(`
		INSERT INTO dbname."bmtest1"("a", "b", "c", "d", "e", "f", "g") 
		VALUES ('${num}--${str[index]}','${str[index+1]}',150833256,${num},'${str[index+2]}','${str[index+3]}','1980-07-13 05:03:0${index}')`
	)
    await new Promise(resolve => setTimeout(resolve, 1e3/qps))
}
	console.timeEnd()
	console.log('count:',count)
	//console.log(sessions)
	connection.close();
}
main()