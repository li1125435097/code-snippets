package main

import (
	"context"
	"fmt"
	"log"
	"os"
	"strconv"
	"time"

	"go-grpc-client/generated/sum"

	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
)

func main() {
	// 设置 gRPC 服务器地址
	serverAddr := "localhost:50051"
	if addr := os.Getenv("GRPC_SERVER_ADDR"); addr != "" {
		serverAddr = addr
	}

	// 解析命令行参数
	args := os.Args[1:]
	if len(args) == 0 {
		fmt.Println("Usage: go run main.go <number1> <number2> ...")
		fmt.Println("Example: go run main.go 1 2 3 4 5")
		os.Exit(1)
	}

	// 连接 gRPC 服务器
	conn, err := grpc.Dial(serverAddr,
		grpc.WithTransportCredentials(insecure.NewCredentials()),
		grpc.WithBlock(),
		grpc.WithTimeout(5*time.Second),
	)
	if err != nil {
		log.Fatalf("连接服务器失败: %v", err)
	}
	defer conn.Close()

	// 创建 gRPC 客户端
	client := sum.NewSumServiceClient(conn)
	start := time.Now()

	// 准备请求
	req := &sum.SumRequest{
		Numbers: args,
	}

	// 调用 gRPC 服务
	ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
	defer cancel()
	resp, err := client.Sum(ctx, req)
	elapsed := time.Since(start)
	fmt.Printf("调用 Sum 服务耗时: %s\n", elapsed)
	if err != nil {
		log.Fatalf("调用 Sum 服务失败: %v", err)
	}

	// 解析结果
	result, err := strconv.ParseInt(resp.Result, 10, 64)
	if err != nil {
		// 如果结果太大，直接输出字符串
		fmt.Printf("计算结果: %s\n", resp.Result)
	} else {
		fmt.Printf("计算结果: %d\n", result)
	}
}