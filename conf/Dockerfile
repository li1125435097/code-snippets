FROM node:lts-alpine

WORKDIR /app
COPY . /app

# RUN npm set registry https://registry.npm.taobao.org
# RUN npm install
# RUN npm run build

# ENV NODE_ENV=production
# RUN npm prune --production

EXPOSE 3000
CMD ["node", "app.min.js"]

# dcoker 构建、导出、导入命令
# "docker_build": "docker build -t light-server .",
# "docker_export": "docker save -o light-server.tar light-server:latest",
# "docker_import": "docker load -i light-server.tar",