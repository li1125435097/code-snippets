int main() {
    printf("主版本号：%d\n", UV_VERSION_MAJOR);
    printf("小版本号：%d\n", UV_VERSION_MINOR);
    printf("补丁版本号：%d\n", UV_VERSION_PATCH);
    printf("全版本号0x%x\n",UV_VERSION_HEX);
    printf("是否为发布版：%d\n", UV_VERSION_IS_RELEASE);
    printf("版本后缀：%s\n", UV_VERSION_SUFFIX);

    printf("函数获取数字版本：%d\n", uv_version());
    printf("函数获取字符串版本：%s\n", uv_version_string());
    return 0;
}