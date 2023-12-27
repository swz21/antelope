#!/bin/bash

load_bpf() {
    cd /home/vagrant/antelope
    mkdir -p /tmp/cgroupv2
    sudo mount -t cgroup2 none /tmp/cgroupv2
    sudo mkdir -p /tmp/cgroupv2/foo
    bash
    source ~/venv/bin/activate
    sudo sh -c "echo $BASHPID >> /tmp/cgroupv2/foo/cgroup.procs"
    sudo bpftool prog load tcp_changecc_kern.o /sys/fs/bpf/tcp_prog
    sudo bpftool cgroup attach /tmp/cgroupv2/foo sock_ops pinned /sys/fs/bpf/tcp_prog
    sudo bpftool prog tracelog
}

get_training_data() {
    cd /home/vagrant/antelope
    bash
    source ~/venv/bin/activate
    sudo python getTrainData.py
}

run_cc_server() {
    cd /home/vagrant/antelope
    bash
    source ~/venv/bin/activate
    sudo python antelope.py
}

run_iperf_server() {
    bash
    iperf3 -s -p 12345 -i 1
}

run_iperf_client() {
    bash
    iperf3 -c 127.0.0.1 -p 12345 -t 5 -P 10 -R
}

run_antelope_training() {
    load_bpf &
    get_training_data &
}

run_antelope_service() {
    load_bpf &
    run_cc_server &
}

for arg in "$@"; do
    case $arg in
        -h|--help)
            echo "Usage: run_exp.sh [options]"
            echo "Options:"
            echo "  -t, --train: get training data"
            echo "  -r, --run: run antelope service"
            exit 0
            ;;
        -t|--train)
            run_antelope_training
            ;;
        -s|--server)
            run_antelope_service
            ;;
        *)
            echo "Unknown option: $arg"
            exit 1
            ;;
    esac
done