# Antelope
Antelope: a system which can adaptively choose the most suitable congestion control mechanism for a certain flow. 

The antelope system is divided into two parts, one is the kernel module and the other is the user_space module.

Before you run antelope, you need to modify the linux kernel so that antelope can get the required information. The modification is shown in linux-4.18.0-147_el8_diff.txt.

Then you need to install bcc, so that getSocketInfo.py use bcc mechanism to read the socket information.

Being able to switch CC mechainsm flexibly is very important for antelope, and ebpf will help us achieve this function.To load the ebpf program into the kernel, you need：
  1: make samples/bpf/
  
  2: mkdir -p /tmp/cgroupv2
 
  3: mount -t cgroup2 none /tmp/cgroupv2
  
  4: mkdir -p /tmp/cgroupv2/foo
  
  5: bash
  
  6: echo $$ >> /tmp/cgroupv2/foo/cgroup.procs
  
  7: ./samples/bpf/load_sock_ops -l /tmp/cgroupv2/foo ./samples/bpf/tcp_changecc_kern.o &

When the preparatory work is completed, you can run the recvAndSetCC.py, then Antelope which try to choose a suitable CC mechanism for different TCP flows according their in-flow statistical information is runing


=======Distributed scene==========

In order to reduce the overhead of machine learning on the end server, we split the recvAndSetCC.py into cc-server.py and recvAndSetCC_distributed.py. Learning cloud server run cc-server.py to choose the suitable CC mechanism. End server run recvAndSetCC_distributed.py to obtain the CC selected by learning cloud server, and perform CC switching.

---

## Project Structure

- `scripts`: all the scripts for install, setup, etc. Rsync to `/home/vagrant/scripts` in the VM.
- `kern_src`: the linux-5.10.179 kernel source code and other kernel related files/patches. Map to `/home/vagrant/kern_src` in the VM.

## Reproducing set up

### Setup development environment on host

Run `setup.sh` by `bash setup.sh` or other similar commands. The script `setup.sh` will help you do the following things:

- Install **vagrant** and **libvirt**.
- Install vagrant plugins: **vagrant-rsync-back**(used to rsync from vm to host)
- Add the user to *libvirt* group and *kvm* group.

Then you need to **logout** and **re-login** to make the settings take effort.  

### Setup up the VM

Build the VM with vagrant-libvirt by `vagrant up`.

This will automatically run `scripts/setup antelope-vm` inside the VM as provision.

Use `vagrant ssh` to login to the VM.

### Setup the bpf environment

Run `scripts/setup bpf` inside the VM.

## Running experiments

Run `scripts/setup antelope-exp` inside the VM.

Use `scripts/run_exp.sh` to run the experiments:

- use `scripts/run_exp.sh -h` for more helpful information.
- use `scripts/run_exp.sh -l` to load the bpf program.
- use `scripts/run_exp.sh -t` to launch the program which will collect the training data.
- use `scripts/run_exp.sh -s` to run the antelope server to switch CCs.
