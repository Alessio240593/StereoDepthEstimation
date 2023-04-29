#! /bin/bash

apt install -y build-essential libssl-dev libffi-dev python3.10-dev

apt install -y python3.10-venv

DIR=env

if [ ! -d  "$DIR" ]; then
	python3.10 -m venv env
fi

source env/bin/./activate

echo 'Installation of dependencies on progress...'
pip3.10  install -r requirements.txt

deactivate

echo "alias python='python3.10'" >> ~/.bashrc

source ~/.bashrc