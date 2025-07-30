const fs = require('fs')

// 角度转百分比
function angleScaleChange(angle){
    return 100-Math.round(Math.sin(parseInt(angle)*Math.PI/180)*100)+'%'
}

function location(index){
    if([5,6].includes(index)) return '4/5'
    else if([11,12].includes(index)) return '3/5'
    else if([14,15].includes(index)) return '3/4'
    else if([17,18].includes(index)) return '2/5'
    else if([23,24].includes(index)) return '1/5'
    else if([36,37].includes(index)) return '外1/5'
    else if([44,45].includes(index)) return '外2/5'
    else if([48,49].includes(index)) return '外1/4'
    else if([52,53,54].includes(index)) return '外3/5'
    else if([63,64,65].includes(index)) return '外4/5'

    if(index < 30) return '较上角度稍边偏'
    else if(index = 30) return '边缘'
    else return '较上角度稍外偏'
}

const x = []
const y = []
const lines = ['| 角度 | 重叠比例 | 瞄准点位置 |','|-------|-------|-------|']
new Array(91).fill(0).forEach((item,index)=>{
    lines.push(`| ${index}° | ${angleScaleChange(index)} | ${location(index)} |`)
    x.push(index)
    y.push(+angleScaleChange(index).slice(0,-1))
})

fs.writeFileSync('./a.md',lines.join('\n'))
console.log(x,y)