#!/bin/sh

set -e

echo "Building"
make -C ebpf re
cargo build

echo "Running libbpf variant"
sudo ./ebpf/build/test_probe_runner

echo "Running aya variant"
sudo ./target/debug/aya_struct_flavor_reloc_repro
