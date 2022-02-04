#!/bin/bash

HOME="/root"
mkdir $HOME
cd $HOME
mkdir workspace
mkdir Downloads

# One line to install tzdata
DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends tzdata

# install mongodb
cd Downloads
wget -qO - https://www.mongodb.org/static/pgp/server-4.4.asc | apt-key add -
echo "deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu bionic/mongodb-org/4.4 multiverse" | tee /etc/apt/sources.list.d/mongodb-org-4.4.list
apt-get update
apt-get install -y mongodb-org
systemctl daemon-reload
systemctl enable mongod
systemctl start mongod

# install minicomda in silent mode from ~
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh -O $HOME/Downloads/miniconda.sh
chmod +x $HOME/Downloads/miniconda.sh
echo "yes" | $HOME/Downloads/miniconda.sh -b -p $HOME/miniconda
$HOME/miniconda/bin/conda env create -f /docker/environment.yml

# replace .bashrc
mv /docker/.bashrc $HOME/.bashrc

# Clean cache to save docker image size
rm -rf $HOME/Downloads/*
apt-get clean
rm -rf /var/lib/apt/lists/*