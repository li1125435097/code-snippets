if test -f "./src/sum.rs"; then
    echo "proto已编译"
else
    bash ./build.sh
    echo "proto编译成功"
fi
cargo run