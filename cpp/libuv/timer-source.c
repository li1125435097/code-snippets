#include "uv.h"
 #include "uv-common.h"
 #include "heap-inl.h"
 #include <limits.h>

//  定时器句柄 (uv_timer_t)
 struct uv_timer_s {
    uv_handle_t handle;          // 基础句柄
    uv_timer_cb timer_cb;        // 定时器回调函数
    uint64_t timeout;            // 绝对超时时间(毫秒)
    uint64_t repeat;             // 重复间隔(毫秒)
    uint64_t start_id;           // 启动ID（用于排序）
    union {
      struct {
        struct heap_node heap;   // 在定时器堆中的节点
      } node;
      struct uv__queue queue;    // 在就绪队列中的节点
    };
  };
 
 // 定义一个函数，用于获取定时器堆
 static struct heap *timer_heap(const uv_loop_t* loop) {
 #ifdef _WIN32
   // 在Windows系统中，返回loop->timer_heap的指针
   return (struct heap*) loop->timer_heap;
 #else
   // 在其他系统中，返回loop->timer_heap的地址
   return (struct heap*) &loop->timer_heap;
 #endif
 }
 
 // 定义一个函数，用于比较两个定时器节点的大小，主要比较超时时间（绝对时间），当超时时间相同时，比较启动ID确保先进先出，保证定时器执行的顺序确定性
 static int timer_less_than(const struct heap_node* ha,
                            const struct heap_node* hb) {
   const uv_timer_t* a;
   const uv_timer_t* b;
 
   // 使用container_of宏获取uv_timer_t结构体指针
   a = container_of(ha, uv_timer_t, node.heap);
   b = container_of(hb, uv_timer_t, node.heap);
 
   // 比较两个定时器的timeout值
   if (a->timeout < b->timeout)
     return 1;
   if (b->timeout < a->timeout)
     return 0;
 
   /* 当两个定时器的timeout值相同时，比较start_id */
   return a->start_id < b->start_id;
 }
 
 // 初始化定时器
 int uv_timer_init(uv_loop_t* loop, uv_timer_t* handle) {
   // 初始化handle
   uv__handle_init(loop, (uv_handle_t*)handle, UV_TIMER);
   // 设置回调函数为NULL
   handle->timer_cb = NULL;
   // 设置超时时间为0
   handle->timeout = 0;
   // 设置重复时间为0
   handle->repeat = 0;
   // 初始化队列
   uv__queue_init(&handle->node.queue);
   // 返回0
   return 0;
 }
 
 // 启动定时器
 int uv_timer_start(uv_timer_t* handle,
                    uv_timer_cb cb,
                    uint64_t timeout,
                    uint64_t repeat) {
   uint64_t clamped_timeout;
 
   // 检查handle是否正在关闭或cb是否为NULL
   if (uv__is_closing(handle) || cb == NULL)
     return UV_EINVAL;
 
   // 停止定时器
   uv_timer_stop(handle);
 
   // 计算定时器的超时时间
   clamped_timeout = handle->loop->time + timeout;
   if (clamped_timeout < timeout)
     clamped_timeout = (uint64_t) -1;
 
   // 设置定时器的回调函数、超时时间和重复时间
   handle->timer_cb = cb;
   handle->timeout = clamped_timeout;
   handle->repeat = repeat;
   /* start_id是timer_less_than()中的第二个比较索引 */
   handle->start_id = handle->loop->timer_counter++;
 
   // 将定时器节点插入堆中
   heap_insert(timer_heap(handle->loop),
               (struct heap_node*) &handle->node.heap,
               timer_less_than);
   uv__handle_start(handle);
 
   return 0;
 }
 
 // 停止定时器
 int uv_timer_stop(uv_timer_t* handle) {
   // 检查定时器是否处于活动状态
   if (uv__is_active(handle)) {
     // 从堆中移除定时器节点
     heap_remove(timer_heap(handle->loop),
                 (struct heap_node*) &handle->node.heap,
                 timer_less_than);
     uv__handle_stop(handle);
   } else {
     // 从队列中移除定时器节点
     uv__queue_remove(&handle->node.queue);
   }
 
   // 初始化定时器节点队列
   uv__queue_init(&handle->node.queue);
   return 0;
 }
 
 // 重新启动定时器
 int uv_timer_again(uv_timer_t* handle) {
   // 检查定时器回调函数是否为NULL
   if (handle->timer_cb == NULL)
     return UV_EINVAL;
 
   // 如果repeat不为0，则重新启动定时器
   if (handle->repeat) {
     uv_timer_stop(handle);
     uv_timer_start(handle, handle->timer_cb, handle->repeat, handle->repeat);
   }
 
   return 0;
 }
 
 // 设置定时器的重复时间
 void uv_timer_set_repeat(uv_timer_t* handle, uint64_t repeat) {
   handle->repeat = repeat;
 }
 
 // 获取定时器的重复时间
 uint64_t uv_timer_get_repeat(const uv_timer_t* handle) {
   return handle->repeat;
 }
 
 // 获取定时器的剩余时间
 uint64_t uv_timer_get_due_in(const uv_timer_t* handle) {
   if (handle->loop->time >= handle->timeout)
     return 0;
 
   return handle->timeout - handle->loop->time;
 }
 
 // 获取下一个定时器的超时时间
 int uv__next_timeout(const uv_loop_t* loop) {
   const struct heap_node* heap_node;
   const uv_timer_t* handle;
   uint64_t diff;
 
   // 获取堆中的最小节点
   heap_node = heap_min(timer_heap(loop));
   if (heap_node == NULL)
     return -1; /* block indefinitely */
 
   // 获取定时器句柄
   handle = container_of(heap_node, uv_timer_t, node.heap);
   if (handle->timeout <= loop->time)
     return 0;
 
   // 计算剩余时间
   diff = handle->timeout - loop->time;
   if (diff > INT_MAX)
     diff = INT_MAX;
 
   return (int) diff;
 }
 
 // 运行定时器
 void uv__run_timers(uv_loop_t* loop) {
   struct heap_node* heap_node;
   uv_timer_t* handle;
   struct uv__queue* queue_node;
   struct uv__queue ready_queue;
 
   // 初始化就绪队列
   uv__queue_init(&ready_queue);
 
   // 遍历堆中的定时器节点，处理已超时的定时器
   for (;;) {
     heap_node = heap_min(timer_heap(loop));
     if (heap_node == NULL)
       break;
 
     // 获取定时器句柄
     handle = container_of(heap_node, uv_timer_t, node.heap);
     if (handle->timeout > loop->time)
       break;
 
     // 从堆中移除并加入就绪队列
     uv_timer_stop(handle);
     uv__queue_insert_tail(&ready_queue, &handle->node.queue);
   }
 
   // 遍历就绪队列中的定时器节点
   while (!uv__queue_empty(&ready_queue)) {
     queue_node = uv__queue_head(&ready_queue);
     uv__queue_remove(queue_node);
     uv__queue_init(queue_node);
     // 获取定时器句柄
     handle = container_of(queue_node, uv_timer_t, node.queue);
 
     // 如果是重复定时器，重新启动
     uv_timer_again(handle);
     // 调用定时器回调函数
     handle->timer_cb(handle);
   }
 }
 
 // 关闭定时器
 void uv__timer_close(uv_timer_t* handle) {
   uv_timer_stop(handle);
 }