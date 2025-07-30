#一、交互式单次tcp服务
# 在8888端口启动tcp服务 可通过telnet和nc localhost 8888访问
nc -l 8888



#二、交互式多次tcp服务
# 在8888端口启动tcp服务，可重复连接
nc -lk 8888



#三、响应数据tcp服务
# 在8888端口启动tcp服务，tcp客户端访问可响应json数据
echo '{"name":"张三"}' > getData
nc -l 8888 < getData



#四、构建http服务
# 在8888端口启动tcp服务，浏览器访问可响应数据
echo 'HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 6

hellow
' > getData
nc -l 8888 < getData



#六、构建http多次服务实现静态网页
# 在8888端口启动tcp服务，浏览器访问可响应数据
echo 'HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 192

<style>@keyframes move1 { from{margin-left: 0;} to{margin-left: 100vw;} } .move{height: 50px;width: 50px;background-color: red;animation: move1 5s linear infinite;} </style><div class="move">
' > getData
while true;do nc -l 8888 < getData;done