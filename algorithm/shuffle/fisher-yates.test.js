function main(len){
    const s = Date.now()
    const arr = new Array(len).fill(0).map((v,i) => i+1)

    for(let i = arr.length-1; i>0; i--){
        const j = Math.floor(Math.random()*(i+1));
        [arr[i],arr[j]] = [arr[j],arr[i]]
    }

    console.log('\x1b[32m%s\x1b[0m', __filename.replace(process.cwd(), 'shuffle') + ' PASS ' + (Date.now() - s) + 'ms');
    return arr
}

module.exports = main