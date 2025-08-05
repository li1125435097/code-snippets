const {FuncParamTest} = require('./build/Release/addon');
const l = console.log;



l(FuncParamTest({a:1},1,2,() => {
    l('callback');
    l(a)
}))

