// 创建默认循环：

int main{
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}

// 创建自定义循环：

int main{
  uv_loop_t* loop = malloc(sizeof(uv_loop_t));
  uv_loop_init(loop);

  uv_run(loop, UV_RUN_DEFAULT);
  uv_loop_close(loop);
  free(loop);
  return 0;
}