int uv_run(uv_loop_t* loop, uv_run_mode mode) {
    int timeout;
    int r;
    int can_sleep;
  
    // 检查loop是否存活
    r = uv__loop_alive(loop);
    // 如果loop不存活，更新时间
    if (!r)
      uv__update_time(loop);
     
    // 历史原因：旧版本先处理定时器，新版本改为轮询后处理
    // 在进入循环之前处理计时器来保持UV_RUN_DEFAULT循环的向后兼容性。正常只需要在轮询后完成后执行计时器一次，以保持常规事件循环的正确执行顺序
    if (mode == UV_RUN_DEFAULT && r != 0 && loop->stop_flag == 0) {
      uv__update_time(loop);
      uv__run_timers(loop);
    }
  
    // 当loop存活且stop_flag为0时，进入循环
    while (r != 0 && loop->stop_flag == 0) {
      // 无待处理的回调（pending_queue）和空闲句柄（idle_handles）时允许休眠
      can_sleep =
          uv__queue_empty(&loop->pending_queue) &&
          uv__queue_empty(&loop->idle_handles);
  
      // 执行已完成 I/O 的回调（如 write_cb）
      uv__run_pending(loop);
      // 运行空闲句柄回调（低优先级任务）
      uv__run_idle(loop);
      // 执行准备阶段回调（通常用于内部状态更新）
      uv__run_prepare(loop);
  
      // 设置超时时间
      timeout = 0;
      // UV_RUN_ONCE 模式且可休眠时，或 UV_RUN_DEFAULT 模式时，计算超时时间。uv__backend_timeout 返回下一个定时器的剩余时间（若无定时器则返回 -1，表示无限等待）。
      if ((mode == UV_RUN_ONCE && can_sleep) || mode == UV_RUN_DEFAULT)
        timeout = uv__backend_timeout(loop); // 基于最快到期定时器计算
  
      // 统计循环次数
      uv__metrics_inc_loop_count(loop);
  
      // 执行I/O轮询（事件循环核心），监听文件描述符（如 socket、文件）的 I/O 事件，触发对应的回调
      uv__io_poll(loop, timeout);
  
      // 处理少量固定数量的即时回调（例如write_cb），限制次数（8次）‌：防止因持续有新回调加入导致事件循环饥饿
      for (r = 0; r < 8 && !uv__queue_empty(&loop->pending_queue); r++)
        uv__run_pending(loop);
  
      // 计算空闲时间占比，在provider_idle_time上运行最后一次更新，以防uv__io_poll因超时而返回，但未收到任何事件。如果provider_entry_time从未设置（如果超时==0）或已更新，则此调用将被忽略，因为收到了事件
      uv__metrics_update_idle_time(loop);
  
     //  运行检查阶段回调（如 setImmediate 的 Node.js 实现）
      uv__run_check(loop);
     //  关闭已标记为终止的句柄（释放资源）
      uv__run_closing_handles(loop);
  
      // 更新时间
      uv__update_time(loop);
      // 定时器触发‌：检查并执行所有到期的定时器回调。
      uv__run_timers(loop);
  
      // 检查loop是否存活
      r = uv__loop_alive(loop);
      // 循环终止条件，如果mode为UV_RUN_ONCE或UV_RUN_NOWAIT，则跳出循环
      if (mode == UV_RUN_ONCE || mode == UV_RUN_NOWAIT)
        break;
    }
  
     // 清除停止标志‌：为下一次循环执行做准备
    if (loop->stop_flag != 0)
      loop->stop_flag = 0;
  
    // 返回循环是否仍有活跃句柄（1 为活跃，0 为终止）
    return r;
  }