class LinkList {
    constructor(val,node) {
        this.prev = node || null;
        this.val = val || 0;
    }
}
// 链表翻转
function main(){
    let head = new LinkList(8);
    let node1 = new LinkList(9,head);
    let node2 = new LinkList(10,node1);
    let node3 = new LinkList(11,node2);
    let node4 = new LinkList(12,node3);
    let node = node4;

    // 链表遍历输出
    while(node.prev != null){
        console.log(node.val);
        node = node.prev;
    }
    console.log(node.val);

    // 反转链表
    let newHead = node4;
    let nPtr = newHead.prev;
    newHead.prev = null;
    
    while(nPtr != null){
        let temp = newHead
        newHead = nPtr;
        nPtr = newHead.prev
        newHead.prev = temp;
    }

    // 反转链表遍历输出
    while(newHead.prev != null){
        console.log(newHead.val);
        newHead = newHead.prev;
    }
    console.log(newHead.val);
}

console.time()
const result = main()
console.timeEnd()
console.log(result);
