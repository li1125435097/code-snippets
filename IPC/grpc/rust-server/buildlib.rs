fn main() -> Result<(), Box<dyn std::error::Error>> {
    // 获取项目根目录
    let manifest_dir = std::env::var("CARGO_MANIFEST_DIR").unwrap();
    let proto_path = format!("{}/proto/sum.proto", manifest_dir);
    
    // 打印调试信息
    println!("cargo:rerun-if-changed={}", proto_path);
    println!("Building protobuf: {}", proto_path);
    
    // 编译 Protobuf
    tonic_build::configure()
        .out_dir("src")  // 输出到新目录
        .compile(&[&proto_path], &[&manifest_dir]);
    
    Ok(())
}