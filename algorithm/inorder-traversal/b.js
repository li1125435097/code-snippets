const len = 10000
console.time()
const arr = Array.from({length:len},(v,i) => i+1)
console.timeEnd()

console.time()
const arr1 = new Array(len).fill(0).map((v,i) => i+1)
console.timeEnd()

console.time()
for(let i = arr.length-1; i>0; i--){
    const j = Math.floor(Math.random()*i);
    [arr[i],arr[j]] = [arr[j],arr[i]]
}
console.timeEnd()

console.log(arr)