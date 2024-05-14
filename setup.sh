#!/bin/bash

echo 'Setup up'

echo 'Checking if vagrant is installed with required version(2.3.1)...'
VAGRANT_VERSION="$(vagrant -v)"
if [[ "$VAGRANT_VERSION" == *"2.3.1" ]]; then
    echo 'Exists vagrant 2.3.1...skip intstall!'
else
    echo 'No exists ...install vagrant 2.3.1!'
    wget -c https://releases.hashicorp.com/vagrant/2.3.1/vagrant_2.3.1-1_amd64.deb
    sudo dpkg -i vagrant_2.3.1-1_amd64.deb
    sudo rm vagrant_2.3.1-1_amd64.deb
fi


echo 'Update apt dependencies'
sudo apt-get update -y
sudo apt install libelf-dev python-dev -y
# echo 'Installing nfs'
# sudo apt-get install nfs-common nfs-kernel-server -y

echo 'Installing vagrant and libvirt...'
curl -O https://raw.githubusercontent.com/vagrant-libvirt/vagrant-libvirt-qa/main/scripts/install.bash
chmod a+x ./install.bash
./install.bash || exit 1
rm ./install.bash

echo 'Installing vagrant plugins...'
vagrant plugin install vagrant-rsync-back


echo 'Grant the user privilege... '
sudo usermod -aG kvm "$USER"
sudo usermod -aG libvirt "$USER"

echo 'Set up done.'
echo 'ATTENTION: Please exit and re-login your account to make the privilege take effect.'
echo 'Then run "vagrant up" and "vagrant ssh" to connect to the guest.'
