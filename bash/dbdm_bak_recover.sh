#!/bin/bash
if [ "$(id -u)" -ne 0 ]; then
    echo "错误： 请使用sudo或root用户运行"
    exit 1;
fi

# 参数检查
if [ $# -lt 1 ]; then
    echo "用法: <export|import> [版本号]"
    exit 1
fi

# 定义工作目录
LOCAL=`pwd`
BIN="/home/dmdba/dmdbms/bin"

OPERATION=$1
VERSION=$2
BACKUP_DIR=$(pwd)
TARGET_DIR="/home/db1/webapp"
DM_USER="SYSDBA"
DM_PWD="SYSDBA"
DM_HOST="localhost"
DM_PORT="5236"

# 获取最大版本号函数
get_max_version() {
    ls v*.tar 2>/dev/null | grep -oE 'v[0-9]+' | cut -c2- | sort -nr | head -1
}

# 清理临时文件
deltf() {
    rm -fr *.dmp mb config.json *.log
}

# 导出功能
export_db() {
    # 确定版本号
    if [ -z "$VERSION" ]; then
        MAX_VER=$(get_max_version)
        VERSION=$((MAX_VER + 1))
        [ -z "$MAX_VER" ] && VERSION=1
    fi

    # 导出schema
    cd $BIN 
    echo "正在导出db1 schema..."
    ./dexp USERID=${DM_USER}/${DM_PWD}@${DM_HOST}:${DM_PORT} SCHEMAS=db1 FILE=db1.dmp LOG=db1_exp.log
    echo "正在导出db2 schema..."
    ./dexp USERID=${DM_USER}/${DM_PWD}@${DM_HOST}:${DM_PORT} SCHEMAS=db2 FILE=db2.dmp LOG=db2_exp.log
    
    # 移动文件，切换路径
    mv *.dmp $LOCAL
    $(deltf)
    cd $LOCAL

    # 备份其他文件
    cp ${TARGET_DIR}/config.json .
    cp -r ${TARGET_DIR}/mb .

    # 打包
    TAR_FILE="v${VERSION}.tar"
    tar -cvf ${TAR_FILE} db1.dmp db2.dmp config.json mb/
    echo "导出完成，版本包: ${TAR_FILE}"
    $(deltf)
}

# 导入功能
import_db() {
    # 确定版本号
    if [ -z "$VERSION" ]; then
        VERSION=$(get_max_version)
        [ -z "$VERSION" ] && { echo "未找到可用版本包"; exit 1; }
    fi

    TAR_FILE="v${VERSION}.tar"
    [ ! -f "$TAR_FILE" ] && { echo "版本包不存在: ${TAR_FILE}"; exit 1; }

    # 解压
    tar -xvf ${TAR_FILE}
    
    # 清空并导入schema
    mv *.dmp $BIN
    cd $BIN
    echo "正在清空并导入db1 schema..."
    ./dimp USERID=${DM_USER}/${DM_PWD}@${DM_HOST}:${DM_PORT} SCHEMAS=db1 FILE=db1.dmp LOG=db1_imp.log TABLE_EXISTS_ACTION=replace
    echo "正在清空并导入db2 schema..."
    ./dimp USERID=${DM_USER}/${DM_PWD}@${DM_HOST}:${DM_PORT} SCHEMAS=db2 FILE=db2.dmp LOG=db2_imp.log TABLE_EXISTS_ACTION=replace
    echo "正在清空摄像头ext_id"
    echo 'UPDATE "db1"."camera" set "ext_id"=NULL;' | ./disql "$DM_USER/$DM_PWD@$DM_HOST:$DM_PORT"
    $(deltf)

    # 覆盖恢复其他文件
    cd $LOCAL
    cp -f config.json ${TARGET_DIR}/
    cp -rf mb ${TARGET_DIR}/
    echo "导入完成，版本: ${VERSION}"
    $(deltf)
}

# 主逻辑
case "$OPERATION" in
    export)
        export_db
        ;;
    import)
        import_db
        ;;
    *)
        echo "无效操作: $OPERATION (支持: export/import)"
        exit 1
        ;;
esac