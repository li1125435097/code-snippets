/**
 * @name 知乎文章生成目录索引md文件
 * @description 浏览器控制台执行该脚本，生成md文件，导入知乎文章，即可自动成目录文章
 * @exe 在知乎内容创作的文章页面执行该脚本，例如 https://www.zhihu.com/people/theanarkh/posts
 * @author 七老九
 * @date 2025-7-28
 */
const l = console.log
// 定义源数据类型
const RawDataT = {
    desc: 'string', 
    authorName: 'string', 
    itemId: 'string', 
    title: 'string'
}

// 代码执行环境选择，浏览器或node，浏览器填true，node填false
const isForeend = typeof window === 'object' ? true : false
// 分类标签
const labels = ['nodejs','node.js','no.js','libuv','v8']
// 延迟时间，单位秒
const delayTime = 2
// 文章介绍字数限制，-1为无限制
const descCount = -1
async function main() {
    l('脚本执开始----------------------------')
    l('代码运行环境是： ', isForeend ? '浏览器' : 'node')
    l('滚动获取数据等待时间： ', delayTime, '秒')
    l('文章分类标签： ', labels)
    l('文章介绍字数限制： ', descCount)
    l('可以在脚本自定义延迟时间和分类标签，分类标签的作用是所有文章分类时，文章标题包含该标签的会被分类到该标签下')
    l('------------------------------------')

    // 前端自动滚动到底获取全部数据
    if(isForeend) await autoScrollToBottom()

    let rawData
    isForeend 
    ? rawData = getRawData() 
    : rawData = require('./data')
    if(rawData.length) l('源数据获取成功')
    else if(isForeend) return alert('源数据获取失败')
    else return console.error('源数据获取失败')

    const classifyData = getClassifyData(rawData,2,true,labels);
    if(classifyData) l('分类数据获取成功')
    else if(isForeend) return alert('分类数据获取失败')
    else return console.error('分类数据获取失败')

    const mdData = getMdData(classifyData)
    if(mdData) l('md数据生成成功')
    else if(isForeend) return alert('md数据生成失败')
    else return console.error('md数据生成失败')

    isForeend 
    ? exportData(mdData).then(() => alert('md文件导出成功'))
    : require('fs/promises').writeFile('./data.md', mdData).then(() => l('md文件写入成功'))
}
main()


/**
 * @name 知乎文章数据获取
 * @description 获取知乎文章数据
 * @param {string} cname - 类名
 * @example 示例所适用的div <div class="ContentItem ArticleItem" data-zop="{&quot;authorName&quot;:&quot;theanarkh&quot;,&quot;itemId&quot;:&quot;60505970&quot;,&quot;title&quot;:&quot;nodejs之setTimeout源码解析&quot;,&quot;type&quot;:&quot;article&quot;}" itemprop="zhihu:article" itemtype="http://schema.org/SocialMediaPosting" itemscope="" data-za-detail-view-path-module="PostItem" data-za-extra-module="{&quot;card&quot;:{&quot;has_image&quot;:false,&quot;has_video&quot;:false,&quot;content&quot;:{&quot;type&quot;:&quot;Post&quot;,&quot;token&quot;:&quot;60505970&quot;,&quot;upvote_num&quot;:12,&quot;comment_num&quot;:0,&quot;publish_timestamp&quot;:1553619013000,&quot;author_member_hash_id&quot;:&quot;316522515b9766d33526e0d84630f8ee&quot;}}}"><h2 class="ContentItem-title"><span><a href="//zhuanlan.zhihu.com/p/60505970" target="_blank" rel="noopener noreferrer" data-za-detail-view-element_name="Title">nodejs之setTimeout源码解析</a></span></h2><div class="ContentItem-meta"><div class="AuthorInfo ArticleItem-authorInfo" itemprop="author" itemscope="" itemtype="http://schema.org/Person"><div class="AuthorInfo"><meta itemprop="name" content="theanarkh"><meta itemprop="image" content="https://pica.zhimg.com/v2-c2d69009e10a36a5fd793cd2e4869bcb_l.jpg?source=d16d100b"><meta itemprop="url" content="https://www.zhihu.com/people/theanarkh"><meta itemprop="zhihu:followerCount"><span class="UserLink AuthorInfo-avatarWrapper"><div class="css-1gomreu"><a href="//www.zhihu.com/people/theanarkh" target="_blank" class="UserLink-link" data-za-detail-view-element_name="User"><img class="Avatar AuthorInfo-avatar css-1hx3fyn" src="https://pica.zhimg.com/v2-c2d69009e10a36a5fd793cd2e4869bcb_l.jpg?source=d16d100b" srcset="https://pica.zhimg.com/v2-c2d69009e10a36a5fd793cd2e4869bcb_l.jpg?source=d16d100b 2x" alt="theanarkh"></a></div></span><div class="AuthorInfo-content"><div class="AuthorInfo-head"><span class="UserLink AuthorInfo-name"><div class="css-1gomreu"><a href="//www.zhihu.com/people/theanarkh" target="_blank" class="UserLink-link" data-za-detail-view-element_name="User">theanarkh</a></div></span></div><div class="AuthorInfo-detail"><div class="AuthorInfo-badge"></div></div></div></div></div><div class="ArticleItem-extraInfo"><span class="css-z4ujak"></span></div></div><meta itemprop="headline" content="nodejs之setTimeout源码解析"><meta itemprop="url" content="//zhuanlan.zhihu.com/p/60505970"><meta itemprop="datePublished" content="2019-03-26T16:50:13.000Z"><meta itemprop="dateModified" content="2019-03-26T16:50:13.000Z"><meta itemprop="commentCount" content="0"><meta itemprop="image"><div class="RichContent is-collapsed"><span><div class="RichContent-inner"><div class="css-376mun"><span id="content"><span class="RichText ztext CopyrightRichText-richText css-1mev9j9" options="[object Object]" data-font-id="" itemprop="articleBody">setTimeout是在系统启动的时候挂载的全局函数。代码在timer.js。 <span data-first-child=""></span><span class="n">function</span> <span class="nf">setupGlobalTimeouts</span><span class="p">()</span> <span class="p">{</span> <span class="k">const</span> <span class="n">timers</span> <span class="o">=</span> <span class="n">NativeModule</span><span class="p">.</span><span class="n">require</span><span class="p">(</span><span class="err">'</span><span class="n">timers</span><span class="err">'</span><span class="p">);</span> <span class="n">global</span><span class="p">.</span><span class="n">clearImmediate</span> <span class="o">=</span> <span class="n">timers</span><span class="p">.</span><span class="n">cl</span>…</span><span id="VirtualCatalogAnchorPoint"></span></span></div><button type="button" class="Button ContentItem-more FEfUrdfMIKpQDJDqkjte Button--plain fEPKGkUK5jyc4fUuT0QP">阅读全文<span style="display: inline-flex; align-items: center;">​<svg width="24" height="24" viewBox="0 0 24 24" class="Zi Zi--ArrowDown ContentItem-arrowIcon" fill="currentColor"><path fill-rule="evenodd" d="M17.776 10.517a.875.875 0 0 1-.248 1.212l-5.05 3.335a.875.875 0 0 1-.964 0L6.47 11.73a.875.875 0 1 1 .965-1.46l4.56 3.015 4.568-3.016a.875.875 0 0 1 1.212.248Z" clip-rule="evenodd"></path></svg></span></button></div></span><div class="ContentItem-actions"><span><span><button aria-label="赞同 12 " aria-live="polite" type="button" class="Button VoteButton FEfUrdfMIKpQDJDqkjte"><span style="display: inline-flex; align-items: center;">​<svg width="10" height="10" viewBox="0 0 24 24" class="Zi Zi--TriangleUp VoteButton-TriangleUp" fill="currentColor"><path fill-rule="evenodd" d="M13.792 3.681c-.781-1.406-2.803-1.406-3.584 0l-7.79 14.023c-.76 1.367.228 3.046 1.791 3.046h15.582c1.563 0 2.55-1.68 1.791-3.046l-7.79-14.023Z" clip-rule="evenodd"></path></svg></span>赞同 12</button></span><button aria-label="反对" aria-live="polite" type="button" class="Button VoteButton VoteButton--down FEfUrdfMIKpQDJDqkjte"><span style="display: inline-flex; align-items: center;">​<svg width="10" height="10" viewBox="0 0 24 24" class="Zi Zi--TriangleDown" fill="currentColor"><path fill-rule="evenodd" d="M13.792 20.319c-.781 1.406-2.803 1.406-3.584 0L2.418 6.296c-.76-1.367.228-3.046 1.791-3.046h15.582c1.563 0 2.55 1.68 1.791 3.046l-7.79 14.023Z" clip-rule="evenodd"></path></svg></span></button></span><div class="ContentItem-action css-0"><button type="button" class="Button FEfUrdfMIKpQDJDqkjte Button--plain Button--withIcon Button--withLabel fEPKGkUK5jyc4fUuT0QP B46v1Ak6Gj5sL2JTS4PY RuuQ6TOh2cRzJr6WlyQp"><span style="display: inline-flex; align-items: center;">​<svg width="1.2em" height="1.2em" viewBox="0 0 24 24" class="Zi Zi--Comment Button-zi t2ntD6J1DemdOdvh5FB4" fill="currentColor"><path fill-rule="evenodd" d="M12 2.75a9.25 9.25 0 1 0 4.737 17.197l2.643.817a1 1 0 0 0 1.25-1.25l-.8-2.588A9.25 9.25 0 0 0 12 2.75Z" clip-rule="evenodd"></path></svg></span>添加评论</button></div><div class="Popover ShareMenu ContentItem-action"><div class="ShareMenu-toggler" id="Popover453-toggle" aria-haspopup="true" aria-expanded="false"><button type="button" class="Button FEfUrdfMIKpQDJDqkjte Button--plain Button--withIcon Button--withLabel fEPKGkUK5jyc4fUuT0QP B46v1Ak6Gj5sL2JTS4PY RuuQ6TOh2cRzJr6WlyQp"><span style="display: inline-flex; align-items: center;">​<svg width="1.2em" height="1.2em" viewBox="0 0 24 24" class="Zi Zi--Share Button-zi t2ntD6J1DemdOdvh5FB4" fill="currentColor"><path d="M19.47 1.914a.8.8 0 0 1 1.204.778l-1.872 16.386a.9.9 0 0 1-1.204.743l-4.615-1.692a.7.7 0 0 0-.831.28l-1.927 3.02c-.43.674-1.474.369-1.474-.43v-3.865a.8.8 0 0 1 .179-.504l5.808-7.148a.595.595 0 0 0-.897-.781l-5.93 6.354a1.1 1.1 0 0 1-1.258.252L2.57 13.46a.8.8 0 0 1-.08-1.415l16.98-10.13Z"></path></svg></span>分享</button></div></div><button type="button" class="Button ContentItem-action FEfUrdfMIKpQDJDqkjte Button--plain Button--withIcon Button--withLabel fEPKGkUK5jyc4fUuT0QP B46v1Ak6Gj5sL2JTS4PY RuuQ6TOh2cRzJr6WlyQp"><span style="display: inline-flex; align-items: center;">​<svg width="1.2em" height="1.2em" viewBox="0 0 24 24" class="Zi Zi--Star Button-zi t2ntD6J1DemdOdvh5FB4" fill="currentColor"><path d="M10.484 3.307c.673-1.168 2.358-1.168 3.032 0l2.377 4.122a.25.25 0 0 0 .165.12l4.655.987c1.319.28 1.84 1.882.937 2.884l-3.186 3.535a.25.25 0 0 0-.063.193l.5 4.733c.142 1.34-1.222 2.33-2.453 1.782l-4.346-1.938a.25.25 0 0 0-.204 0l-4.346 1.938c-1.231.549-2.595-.442-2.453-1.782l.5-4.733a.25.25 0 0 0-.064-.193L2.35 11.42c-.903-1.002-.382-2.604.937-2.884l4.655-.987a.25.25 0 0 0 .164-.12l2.378-4.122Z"></path></svg></span>收藏</button><button type="button" class="Button ContentItem-action FEfUrdfMIKpQDJDqkjte Button--plain fEPKGkUK5jyc4fUuT0QP"><span style="display: inline-flex; align-items: center;">​<svg width="1.2em" height="1.2em" viewBox="0 0 24 24" class="ZDI ZDI--FlagFill24" fill="currentColor"><path d="M12.842 4.421c-1.86-1.24-3.957-1.408-5.798-1.025-1.827.38-3.467 1.313-4.47 2.381a.75.75 0 0 0-.171.732l4.44 14.546a.75.75 0 1 0 1.434-.438l-1.08-3.542c.025-.018.053-.036.083-.054.298-.184.801-.415 1.568-.523 1.386-.197 2.307.129 3.341.543l.187.075c1.005.405 2.161.872 3.791.804 1.401-.003 2.707-.45 3.67-1.015.483-.284.903-.612 1.212-.953.284-.312.581-.752.581-1.255V5.046a.75.75 0 0 0-1.17-.622c-1.82 1.23-4.881 1.823-7.618-.003Z"></path></svg></span> 举报</button></div></div></div>
 * @returns {RawDataT[]}
 */
function getRawData(cname='ContentItem ArticleItem'){
    const divElements = document.getElementsByClassName(cname)
    let rawData = Array.from(divElements).map(v => Object.assign({desc: v.innerText.split('\n')[2], time: JSON.parse(v.dataset.zaExtraModule)?.card.content.publish_timestamp},JSON.parse(v.dataset.zop) || {}))
    if(!rawData.length) rawData = getSelfRawData()
    return rawData
}

/**
 * @name 获取自己的知乎文章数据
 */
function getSelfRawData(cname='CreationManage-CreationCard'){
    const rawData = Array.from(document.getElementsByClassName(cname))
    .map(v => 
        Array.from(v.childNodes).slice(0,2)
        .map(v => 
            [[v.href],Array.from(v.childNodes)
            .map(v => 
                v.dataset.tooltip || v.textContent
            )])
        .flat(Infinity))
    .map(v => {
        return {
            desc:v[4],
            title:v[1],
            itemId:v[0].split('/').pop(),
            time:parseToTimestamp(v[2])
        }
    })    
    return rawData
}

/**
 * @name 源数据生成md字符串
 * @description 源数据生成md字符串
 * @param {RawDataT[][]} rdata - 分类源数据
 * @param {number} tlevel - 标题等级，默认1
 * @returns {string}
*/
function getMdData(rdata, tlevel=1){
    let mdData = []
    const len = rdata.length
    rdata.map((v1,i1) => {
        if(!v1.length) return
        mdData.push(`${'#'.repeat(tlevel)} ${numToChinese(i1+1)}、${len-1 === i1 ? '其他' : classifyTitleAnaly(v1) || '其他'}`)
        v1.map((v,i) => {
            mdData.push(`${'#'.repeat(tlevel+1)} ${i+1}. ${v.title}`)
            mdData.push(`- **文章介绍** \`${v.desc.slice(0,descCount)}\``)
            mdData.push(`- **发表时间** ${new Date(v.time).toLocaleString()}`)
            mdData.push(`- **直达链接** [${v.title}](https://zhuanlan.zhihu.com/p/${v.itemId})`) 
        })
    })
    return mdData.join('\n')
}

/**
 * @name 源数据分类
 * @description 源数据分类
 * @param {RawDataT[]} rawData - 源数据
 * @param {number} prefixRepetNum - 前缀重复次数，默认2
 * @param {boolean} otherMerge - 其他合并，默认true
 * @param {string[]} labels - 分类标签，有该标签数据直接分类到该标签
 * @returns {RawDataT[][]}
 */
function getClassifyData(rawData, prefixRepetNum=2, otherMerge=true, labels){
    const result = []
    const other = []
    let tmp = []
    let prefix = ''
    const isLabel = Array.isArray(labels) && labels.length > 0
    const labelMap = {}
    const labelPatternMap = {}

    // 数据排序
    const ascRawData = rawData.sort((a,b) => a.title.localeCompare(b.title))
    // 初始化labelMap和labelPatternMap
    if(isLabel){
        labels.forEach(v => {
            labelMap[v] = []
            labelPatternMap[v] = new RegExp(v,'ig')
        })
    }

    for(let i=0; i<ascRawData.length; i++){
        // 标签分类处理
        if(isLabel){
            const label = labels.find(v => ascRawData[i].title.match(labelPatternMap[v]))
            if(label){
                labelMap[label].push(ascRawData[i])
                continue
            }
        }

        // 分类开始处理
        if(!prefix){
            tmp.push(ascRawData[i])
            prefix = ascRawData[i].title.slice(0, prefixRepetNum).toLowerCase()
        }
        // 分类结束处理
        else if(ascRawData[i].title.slice(0, prefixRepetNum).toLowerCase() !== prefix){
            if(otherMerge && tmp.length === 1) other.push(tmp[0])
            else result.push(tmp)
            tmp = []
            prefix = ''
        }
        // 分类合并处理
        else tmp.push(ascRawData[i])
    }
    result.push(other)

    // 反向插入标签分类数据
    if(isLabel) Object.values(labelMap).reverse().forEach(v => v.length && result.unshift(v))

    return result;
}

/**
 * @name 分类一级标题分析
 * @param {*} rawData 
 * @returns 
 */
function classifyTitleAnaly(rawData){
    const strings = rawData.map(v => v.title);
    const prefixCount = new Map();

    for (const str of strings) {
        for (let i = 1; i <= str.length; i++) {
            const prefix = str.slice(0, i);
            prefixCount.set(prefix, (prefixCount.get(prefix) || 0) + 1);
        }
    }

    // 找出出现次数 ≥ 2 的最长前缀
    let longest = '';
    for (const [prefix, count] of prefixCount.entries()) {
        if (count >= 2 && prefix.length > longest.length) {
            longest = prefix;
        }
    }

    return longest;
}

/**
 * @name 数字转中文
 * @param {number} num - 数字
 * @returns 
 */
function numToChinese(num) {
    if (typeof num !== 'number' || num < 1 || num > 999 || !Number.isInteger(num)) {
        throw new Error("请输入 1 到 999 之间的整数");
    }

    const chineseDigits = ['零', '一', '二', '三', '四', '五', '六', '七', '八', '九'];
    const chineseUnits = ['', '十', '百'];

    let result = '';

    // 百位
    const bai = Math.floor(num / 100);
    if (bai > 0) {
        result += chineseDigits[bai] + chineseUnits[2];
    }

    // 十位
    const shi = Math.floor((num % 100) / 10);
    const ge = num % 10;

    if (shi > 0) {
        // 特别处理：10-19 不说“一十”，而是“十”、“十一”...
        if (bai === 0 && shi === 1 && ge < 10 && ge >= 0) {
            result = chineseUnits[1]; // 10 -> 十, 11 -> 十一 ...
            if (ge > 0) {
                result += chineseDigits[ge];
            }
            return result;
        } else {
            result += chineseDigits[shi] + chineseUnits[1];
        }
    } else if (shi === 0 && ge > 0 && (bai > 0 || result !== '')) {
        // 处理如 101 中的 "零一"
        result += chineseDigits[0];
    }

    // 个位
    if (ge > 0) {
        result += chineseDigits[ge];
    }

    return result;
}

/**
 * @name 导出数据
 * @description 导出数据
 * @param {string} rawData - md数据
 */
function exportData(mdData){
    const blob = new Blob([mdData], {type: 'text/markdown'});
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'artIndex.md';
    a.click();
    return {then:new Function()}
}

/**
 * @name 自动滚动到底部脚本
 * @param {*} delay - 滚动间隔时间
 * @returns {Promise}
 */
function autoScrollToBottom(delay = 2000) {
    let timer
    let bottomNum = 0
  
    return new Promise((resolve, reject) => {
      // 检查是否到达底部
      function isAtBottom() {
        return window.innerHeight + window.scrollY >= document.body.scrollHeight - 50;
      }
      
      // 滚动到底部
      function scrollToBottom() {

        if(bottomNum > 3){
          clearInterval(timer)
          resolve(true)
          console.log('已滚动到底部，开始获取数据')
          return
        }

        if(isAtBottom()){
            bottomNum++
            l('滚动到底判断',bottomNum)
            return
        }
        l('滚动数据获取中......')
        bottomNum = 0
        window.scrollTo(0, document.body.scrollHeight);
      }
      
      // 初始化滚动
      scrollToBottom();
      // 自动滚动循环
      timer = setInterval(scrollToBottom, delay)
    })
    
  }
  
/**
 * @name 解析发布时间成时间戳
 * @param {string} str 
 * @returns {number}
 */
  function parseToTimestamp(str) {
    // 提取日期时间部分，匹配两种格式
    const regexWithYear = /发布于\s+(\d{4})-(\d{2})-(\d{2})\s+(\d{2}):(\d{2})/;
    const regexWithoutYear = /发布于\s+(\d{2})-(\d{2})\s+(\d{2}):(\d{2})/;

    let match = str.match(regexWithYear);
    if (match) {
        const year = parseInt(match[1], 10);
        const month = parseInt(match[2], 10) - 1; // 月份从0开始
        const day = parseInt(match[3], 10);
        const hour = parseInt(match[4], 10);
        const minute = parseInt(match[5], 10);
        return new Date(year, month, day, hour, minute).getTime();
    }

    match = str.match(regexWithoutYear);
    if (match) {
        const currentYear = new Date().getFullYear();
        const month = parseInt(match[1], 10) - 1;
        const day = parseInt(match[2], 10);
        const hour = parseInt(match[3], 10);
        const minute = parseInt(match[4], 10);
        const date = new Date(currentYear, month, day, hour, minute);

        // 如果生成的日期在当前时间之后，可能是去年的（如：今天是2025年，但日期是12-31）
        const now = new Date();
        if (date > now) {
            // 回退一年
            date.setFullYear(currentYear - 1);
        }

        return date.getTime();
    }

    throw new Error('不支持的日期格式');
}