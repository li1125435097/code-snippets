import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.lang.*;
import java.util.Timer;
import java.util.TimerTask;

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
 * 运行时需声明驱动包路径  `java -cp "/opt/dmdbms/drivers/jdbc/DmJdbcDriver18.jar" test.java`
 */
public class DMDBExample {
    public static void main(String[] args){
        // 出入记录总数
        int count = 50;
        // 每秒插入几条
        double qps = 0.2;

        Timer timer = new Timer();
        InsertTimerTask task = new InsertTimerTask(count);
        double interval = 1e3/qps;
        timer.scheduleAtFixedRate(task,0,(int)interval);

        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            task.Destory();
            System.out.println("进程退出");
        }));
    }
}

class InsertTimerTask extends TimerTask {
    private String url = "jdbc:dm://ip:5236/dbname?loginEncrypt=false";
    private String user = "dev";
    private String pwd = "123456";
    private Connection conn;
    private Statement stmt;
    private ResultSet rs;
    private int num = 0;
    private String[] str = {"liangjiang","哦第四耐，法努力科技","sunchengbin","纯与的老鈤","forovertheworld","按照 中队>低慢小>独立摄像头 排序","emeisha","重庆老何","奥巴马吃桑巴"};
    private int index;
    private int count;

    public InsertTimerTask(int count){
        this.count = count;
        try {
            Class.forName("dm.jdbc.driver.DmDriver");
            this.conn = DriverManager.getConnection(url,user,pwd);
            this.stmt = conn.createStatement();
            System.out.println("构造函数执行完成");
        } catch (Exception e){
            e.printStackTrace();
        }
        
    }

    public void Destory(){
        try{
            this.rs.close();
            this.stmt.close();
            this.conn.close();
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public void run(){
        this.num++;
        this.index = this.num%9;
        String sql = "INSERT INTO YISHI.\"bmtest1\"(\"a\", \"b\", \"c\", \"d\", \"e\", \"f\", \"g\")" +
            "VALUES ('"+
            this.num + "==="+this.str[this.index]+
            "','"+
            this.str[(this.index+1)%9]+
            "',"+
            1833256+
            ","+
            1+
            ",'"+
            this.str[(this.index+2)%9]+
            "','"+
            this.str[(this.index+3)%9]+
            "','"+
            "1980-07-13 05:03:0"+this.index+
            "')";
        
        try {
            this.rs = this.stmt.executeQuery(sql);
        } catch (Exception e){
            e.printStackTrace();
        }
        System.out.println(this.num);
        if(this.count == this.num){
            Runtime.getRuntime().exit(0);
        }
    }
}

// InsertThread t = new InsertThread();
// t.start();
class InsertThread extends Thread {
    public void run(){
        System.out.println("aa");
    }
}