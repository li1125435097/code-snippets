if test -f "./generated/sum/sum.pb.go"; then
    echo "proto已编译"
else
    bash ./proto-build.sh
    echo "proto编译成功"
fi
go run . 30