const cp = require('child_process')
const fs = require('fs')

// 混淆
cp.exec('javascript-obfuscator tq.js --compact true --self-defending false -o tq.min.js', (err, stdout, stderr) => {
    if(!err) {
        console.log('混淆成功')
        let html = fs.readFileSync('tq.html', 'utf-8').toString()
        html = html.replace('<script src="./tq.js"></script>', `<script>${fs.readFileSync('./tq.min.js')}</script>`)
        fs.writeFileSync('tq.min.html', html, 'utf-8')
        console.log('混淆合包成功')
        cp.exec('html-minifier-terser  --collapse-whitespace --remove-comments --minify-js --minify-css --decode-entities --use-short-doctype tq.min.html -o tq.min.html', (err, stdout, stderr) => {
            if(!err) {
                console.log('html压缩成功')
            }else {
                console.log('html压缩失败')
            }
        })
        cp.exec('rm tq.min.js')
    }else {
        console.log('混淆失败')
    }
})