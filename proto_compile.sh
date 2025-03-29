#!/bin/bash
# directory of protoc: X:/GithubProjects/vcpkg/packages/protobuf_x64-windows/tools/protobuf

#where protobuf.exe and grpc_cpp_plugin.exe are
PROTOC_DIR="C:\Users\samoh\vcpkg\packages\protobuf_x64-windows\tools\protobuf"
GRPC_CPP_EXE="C:\Users\samoh\vcpkg\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe"

#where your .proto files are
SRC_DIR="D:\GitRepository\Scene_Showcase_PS4"
DST_DIR="D:\GitRepository\Scene_Showcase_PS4"

echo "Src Directory: $SRC_DIR"
echo "Dst Directory: $DST_DIR"

$PROTOC_DIR/protoc --proto_path=$SRC_DIR --cpp_out=$DST_DIR --grpc_out=$DST_DIR --plugin=protoc-gen-grpc=$GRPC_CPP_EXE $SRC_DIR/LevelData.proto

$SHELL