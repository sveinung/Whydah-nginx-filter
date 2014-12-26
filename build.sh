#!/bin/bash

set -e

NGINX_DOWNLOAD_DIR=nginx-1.6.0
NGINX_TARBALL=${NGINX_DOWNLOAD_DIR}.tar.gz

pushd /vagrant

mkdir -p output

pushd output

wget --no-clobber http://nginx.org/download/$NGINX_TARBALL

if [ ! -d $NGINX_DOWNLOAD_DIR ]; then
    tar xvf $NGINX_TARBALL
fi

pushd $NGINX_DOWNLOAD_DIR

./configure \
    --without-http_rewrite_module \
    --add-module=/vagrant/src
make
sudo make install

sudo service nginx restart

popd
popd
popd
