use tonic::{transport::Server, Request, Response, Status};
use num_bigint::BigInt;
use std::str::FromStr;
use std::time::Instant;

// 包含生成的代码
pub mod sum;
use sum::{
    sum_service_server::{SumService, SumServiceServer},
    SumRequest, SumResponse
};

#[derive(Debug, Default)]
struct SumServiceImpl;

#[tonic::async_trait]
impl SumService for SumServiceImpl {
    async fn sum(
        &self,
        request: Request<SumRequest>,
    ) -> Result<Response<SumResponse>, Status> {
        let start = Instant::now();
        let req = request.into_inner();
        
        // 将字符串解析为大整数
        let numbers: Result<Vec<u32>, _> = req.numbers
            .into_iter()
            .map(|s| s.parse::<u32>())
            .collect();
        
        let numbers = numbers.map_err(|_| Status::invalid_argument("Invalid number format"))?;
        
        println!("Received numbers: {:?}", numbers);
        // 计算总和
        // let sum: BigInt = numbers.into_iter().sum();
        // let sum = fibonacci(numbers[0]);
        let sum = 60;
        let duration = start.elapsed();
        println!("耗时: {:?}", duration);
        Ok(Response::new(SumResponse {
            result: sum.to_string()
        }))
    }
}

fn fibonacci(n: u32) -> u32 {
    match n {
        0 => 0,
        1 => 1,
        _ => fibonacci(n - 1) + fibonacci(n - 2),
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let addr = "[::1]:50051".parse()?;
    let service = SumServiceImpl::default();

    println!("Rust gRPC Server listening on {}", addr);

    Server::builder()
        .add_service(SumServiceServer::new(service))
        .serve(addr)
        .await?;

    Ok(())
}