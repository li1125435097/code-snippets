// echarts tooltip提示formatter函数权威指南 https://zhuanlan.zhihu.com/p/431669979
let chartQueue = []           // echarts实例队列

//- 一个带联动的初始化函数 
$(function(){
      let priceRange = [{ value: 1048, name: '1-3' },{ value: 484, name: '2-6' },{ value: 580, name: '6-8' },{ value: 735, name: '8-10' },{ value: 300, name: '10+' }]
      let pie = renderPieChart('#pricePie','比例图',priceRange,'比例图')
      let bar = renderBarChart('#priceBar',priceRange.map(v=>v.name),[priceRange.map(v=>v.value)],'数量图',['单价'],'bar')
      linkActive(pie,bar)
      renderBarChart('#latRateBar',[465,54,5,45,54,5,45,45,45],[[545,564,564,654,56654,5665,4]],'示率图',['好地方'],'line')
      renderBarChart('#latBar',[465,54,5,45,54,5,45,45,45],[[545,564,564,654,56654,5665,4]],'收益图',['好地方'],'line')
      chartResize()
    })


//- 图表尺寸自适应函数
function chartResize(){
  window.addEventListener("resize",function (){
     chartQueue.map(v=>{v.resize()})
  })
}

//- 饼图联动条图
    function linkActive(pie,bar){
      pie.on('mouseover',function(event){bar.dispatchAction({type:'highlight',seriesIndex:0,dataIndex:event.dataIndex})})
      pie.on('mouseout',function(event){bar.dispatchAction({type:'downplay',seriesIndex:0,dataIndex:event.dataIndex})})
      bar.on('mouseover',function(event){pie.dispatchAction({type:'highlight',seriesIndex:0,dataIndex:event.dataIndex})})
      bar.on('mouseout',function(event){pie.dispatchAction({type:'downplay',seriesIndex:0,dataIndex:event.dataIndex})})
    }


//- 饼图渲染
    function renderPieChart(ele, name, data=[], title){
      let dom = $(ele)[0]      
      if(!ele) return err.push({errmsg:'图表渲染找不到容器',ele:ele})
      let myChart = echarts.init(dom)
      let option = {
        title: {text:title,top:'2%',left: 'center'},
        tooltip: {trigger: 'item'},
        series: [{
          name: name,
          type: 'pie',
          radius: '50%',
          data: data,
          emphasis: {itemStyle: {shadowBlur: 10,shadowOffsetX: 0,shadowColor: 'rgba(0, 0, 0, 0.5)'}}
        }]
      }
      myChart.setOption(option)
      chartQueue.push(myChart)
      return myChart
    }

    //- 条形图渲染，支持多条线渲染
    function renderBarChart(ele, xData, yDatas=[], title='title',lineName=[], lineType){
        let dom = $(ele)[0]      
        if(!ele) return err.push({errmsg:'图表渲染找不到容器',ele:ele})
        let myChart = echarts.init(dom)
        let option = {
          title: {text: title,top: '2%',left:'center'},
          color: ['#6597f6','#76dab0','#687897','#f5c041','#e97962','blue','red','purple','green','pink','green'],
          tooltip: {trigger: 'axis',backgroundColor:'#fff',textStyle:{color:'#777',fontSize:'10',fontWeight:600},borderColor:'#ececec',borderWidth:1,padding:10},
          grid: {left: '10%',right: '8%',bottom: '8%'},
          legend: {data: lineName,right:'20%',top:'10%'},
          xAxis: {type: 'category',data: xData,axisLabel:{interval:0,fontSize:'10',color:'#888'},axisLine:{show:false},axisTick:{show:false}},
          yAxis: {type: 'value',boundaryGap:['10%','10%'],axisLabel:{interval:0,fontSize:'10',color:'#888',formatter: function(value) {return (Math.abs(value)>10000 ? value/1000 + 'k' : value)}},axisLine:{show:false},axisTick:{show:false},splitNumber:5},
          series: []
        }
        option.series = []
        yDatas.map((v,i) => {option.series.push({data: v,type:lineType,name:lineName[i],emphasis:{itemStyle:{color: '#00a500'}}})})
        myChart.setOption(option)
        chartQueue.push(myChart)
        return myChart
      }

       //- 柱状折线融合图表渲染函数
    function renderChart(ele, barData=[], lineData=[], title='title', barName='bar', lineName='line', unit=''){
        let dom = $(ele)[0]      
        if(!ele) return err.push({errmsg:'图表渲染找不到容器',ele:ele})
        //- if(unit == 'k'){barData = barData.map(v=>{return Math.round(v/1000)})}
        let myChart = echarts.init(dom)
        let xData = Array(24).fill(0).map((v,i)=>{return i.toString().padStart(2,'0')+'时'})
        let blueDiv = `<span style="display:inline-block;margin-right:5px;margin-top:10px;border-radius:5%;width:10px;height:10px;left:5px;background-color:rgb(101,151,246);"></span>`
        let greenDiv = `<span style="display:inline-block;margin-right:5px;border-radius:5%;width:10px;height:2px;left:5px;background-color:rgb(118,218,176);vertical-align: middle;"></span>`
        let option = {
          title: {text: title,top: '-2%'},
          color: ['#6597f6','#76dab0'],
          tooltip: {trigger: 'axis',backgroundColor:'#fff',textStyle:{color:'#777',fontSize:'10',fontWeight:600},formatter:function(params) {let line = params[1];let bar = params[0];return `${line.name}</br> ${blueDiv}${bar.seriesName}:&emsp;&emsp;${trans(bar.data)}${unit}</br>${greenDiv}${line.seriesName}:&emsp;&emsp;${line.data}%`},borderColor:'#ececec',borderWidth:1,padding:10},
          grid: {left: '5%',right: '8%',bottom: '8%'},
          legend: {data: [barName, lineName],right:'20%'},
          xAxis: {type: 'category',data: xData,axisLabel:{interval:0,fontSize:'10',color:'#888'},axisLine:{show:false},axisTick:{show:false}},
          yAxis: [
            {type: 'value',boundaryGap:['0%','10%'],axisLabel:{interval:0,fontSize:'10',color:'#888',formatter: function(value) {return (Math.abs(value)>10000 ? value/1000 + 'k' + unit : value + unit)}},axisLine:{show:false},axisTick:{show:false},splitNumber:5},
            {type: 'value',boundaryGap:['10%','10%'],axisLabel:{interval:0,fontSize:'10',color:'#888',formatter:'{value}%'},axisLine:{show:false,onZero:true},axisTick:{show:false},splitLine:{show:false},splitNumber:5}
          ],
          series: [
            {data: barData,type: 'bar',name: barName},
            {data: lineData,type: 'line',name: lineName,yAxisIndex: 1}
          ]
        }
        myChart.setOption(option)
        chartQueue.push(myChart)
      }

    //   饼图自定义tip
      function getOptionp(color=['#aaa','#ddd'],name='共计跟进',unit='个',data=[11,89]){
        let option = {
          color:color,
          series: [{
            type: 'pie',radius: ['90%', '100%'],label: {show: true,position: 'center',formatter: ['{c|}','{a|{c}}','{d|}',`{b|${name}\n(${unit})}`,].join('\n'),
            rich: {a: {fontSize:22},b: {fontSize:12,color:'#999999',lineHeight:18},c: {lineHeight:10},d: {lineHeight:6}}},
            data: [{value:data[0]},{value:data[1]}]
          }]
        }
        return option
      }
    