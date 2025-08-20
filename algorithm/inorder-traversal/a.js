class TreeNode {
    constructor(val, left, right) {
        this.val = (val===undefined ? 0 : val)
        this.left = (left===undefined ? null : left)
        this.right = (right===undefined ? null : right)
    }

    static minValue(root){
        let min = root.val
        while(root.left){
            min = root.left.val
            root = root.left
        }
        return min
    }

    static maxValue(root){
        let max = root.val
        while(root.right){
            max = root.right.val
            root = root.right
        }
        return max
    }

    // 递归遍历，sort: '先序':1 | '中序':2 | '后序':3
    static recursionTraversal(root, cb,sort=1) {
        const traverse = (node) => {
            if (!node) return;
            if (sort === 1) cb(node.val);
            traverse(node.left);
            if (sort === 2) cb(node.val);
            traverse(node.right);
            if (sort === 3) cb(node.val);
        };
        traverse(root);
    }

    // 栈中序遍历
    static stackTraversal(root) {
        const result = new Array();
        const stack = new Array();
        let current = root;
    
        while (current !== null || stack.length > 0) {
            // 遍历到最左节点，沿途节点入栈
            while (current !== null) {
                stack.push(current);
                current = current.left;
            }
            
            // 弹出栈顶节点并访问
            current = stack.pop();
            // result.push(current.val);
            
            // 转向右子树
            current = current.right;
        }
    
        return result;
    }
}
function main(){
    const max = 100000000
    let root = new TreeNode(Math.round(max/2));
    let arr = generateRandomArray(5000000,0,max)

    // 构造二叉排序树
    arr.map(v => {
        let node = root
        let insert = false
        while(!insert){
            if(v < node.val) node.left && (node = node.left) || (insert = true, node.left = new TreeNode(v))
            else node.right && (node = node.right) || (insert = true, node.right = new TreeNode(v))
        }
    })
    
    // 求极值
    console.time('minValue')
    console.log('minValue: ',TreeNode.minValue(root));
    console.timeEnd('minValue')
    console.time('maxValue')
    console.log('maxValue: ',TreeNode.maxValue(root));
    console.timeEnd('maxValue')

    // 中序递归遍历按序列输出
    console.time('recursionTraversal')
    TreeNode.recursionTraversal(root,new Function(),2)
    console.timeEnd('recursionTraversal')

    // 中序栈遍历按序列输出
    console.time('stackTraversal')
    TreeNode.stackTraversal(root)
    console.timeEnd('stackTraversal')
    
}


/**
 * 生成指定长度的乱序整数数组
 * @param {number} length - 数组长度
 * @param {number} [min=0] - 最小值（包含）
 * @param {number} [max=100] - 最大值（包含）
 * @returns {number[]} 乱序整数数组
 */
function generateRandomArray(length, min = 0, max = 100) {
    // 参数校验
    if (length <= 0) throw new Error('Length must be positive');
    if (min > max) throw new Error('Min cannot be greater than max');

    // 生成有序数组
    const arr = Array.from({length}, (_, i) => i + min);
    
    // Fisher-Yates洗牌算法实现乱序
    for (let i = arr.length - 1; i > 0; i--) {
        const j = Math.floor(Math.random() * (i + 1));
        [arr[i], arr[j]] = [arr[j], arr[i]];
    }
    
    // 截取指定长度并调整数值范围
    return arr.slice(0, length).map(num => 
        Math.floor(Math.random() * (max - min + 1)) + min
    );
}

main()