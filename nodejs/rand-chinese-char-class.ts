class MockData{
    /**
     * 获取随机个汉字
     * @param num 
     * @returns 
     */
    static randomText(num:number){ 
        let words:string[]
        words = []
        for(let i=0;i<num;i++){
            eval( "words.push(" +  '"\\u' + (Math.round(Math.random() * 20901) + 19968).toString(16)+'")')//生成随机汉字
        }
        return words.join("");
    }

    /**
     * 获取随机个字母
     * @param num 
     */
    static randomLetter(num:number){
        return Math.random().toString(36).slice(~num+1)
    }

    /**
     * 获取随机数
     * @param max 
     */
    static randomNum(max:number){
        return Math.floor(Math.random()*max)
    }

}