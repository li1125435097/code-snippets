// 数组中寻找两个数下标，使得它们的和等于给定的值
function main(arr,num){
    let map = new Map();
    for(let i = 0; i < arr.length; i++){
        let diff = num - arr[i];
        if(map.has(diff)) return [map.get(diff),i];
        map.set(arr[i],i);
    }
    return [];
}

console.time()
const result = main([2,7,2,11,15,56],58)
console.timeEnd()
console.log(result);
