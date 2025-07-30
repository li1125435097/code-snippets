/**
 * auth: me
 * data：2023-5-19
 * func：写一个轻量日志管理模块，用于统一日志管理
 * note
 *  */
export class Debug{
    static isPrint:boolean
    static color:string
    static filter:Function

    /**
     * 
     * @param value 设置全局是否打印输出
     */
    static setPrint(value:boolean):void{
        Debug.isPrint = value
        if(value) Debug.log('Debug','全局常规日志输入已开启','green')
        else Debug.log('Debug','全局常规日志输入已关闭','red')
    }

    /**
     * 
     * @param value 设置全局日志输出颜色  'black', 'red', 'green', 'yellow', 'blue', 'magenta', 'cyan', 'white'
     */
    static setColor(color:string):void{
        Debug.color = color
    }

    /**
     * 常规日志输出
     * @param tag 标题，一般赋值类名或函数名
     * @param log 要输出的日志
     * @param color 日志颜色，默认随即颜色
     */
    static log(tag:string, log:any, color?:string){
        if(!Debug.isPrint) return
        if(this.filter && this.filter(...arguments)) return
        color = color || Debug.color

        let COLOR_CODE = ['black', 'red', 'green', 'yellow', 'blue', 'magenta', 'cyan', 'white'].indexOf(color)
        if(COLOR_CODE === -1) COLOR_CODE = Math.ceil(Math.random()*6)
        if(COLOR_CODE ===1) COLOR_CODE = 2  // 常规日志取消红色输出

        if(typeof log === 'object') log = JSON.stringify(log)
        const time = new Date().toLocaleString()
        const TITLE_STR = tag ? `\x1b[4${COLOR_CODE};30m ${tag} ${time} \x1b[0m ` : ''
        console.log(`\n${TITLE_STR}\x1b[3${COLOR_CODE}m${log}\x1b[;0m\n`)
    }

    /**
     * 错误日志输出，直接输出，不受其他变量控制
     * @param tag 标题，一般赋值类名或函数名
     * @param log 要输出的日志
     */
    static err(tag:string, log:any){
        if(typeof log === 'object') log = JSON.stringify(log)
        const time = new Date().toLocaleString()
        const TITLE_STR = tag ? `\x1b[41;30m ${tag} [ERROR] ${time} \x1b[0m ` : ''
        console.log(`\n${TITLE_STR}\x1b[31m${log}\x1b[;0m\n`)
    }

    /**
     * 设置日志过滤器，应用于常规日志
     * @param callback
     */
    static setFilter(callback:FilterFn) :void{
        this.filter = callback
    }

}

interface FilterFn{
    (tag:string, log:any, color?:string) :boolean 
}


/**
 * @name 调用示例
// 在main文件初始化日志模块
Debug.setPrint(true);


// 控制器调用
import { Debug } from '../../../comm/debug'
export class UserService extends BaseService {

  private tag = 'UserService'
  private log = (log:any, mark?:string) => Debug.log(`${this.tag} [${mark || ''}]`, log)
  
  @('Post','/create')
  create(body:any){
      this.log(body, 'create1')
      // todo
      this.log(result, 'create2')
  } 
}

*/