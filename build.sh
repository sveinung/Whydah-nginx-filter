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
    --with-debug \
    --without-http_rewrite_module \
    --add-module=/vagrant/src
make
sudo make install

popd

pushd /vagrant/provision/

sudo cp nginx /etc/init.d/
sudo chmod +x /etc/init.d/nginx

sudo cp nginx.conf /usr/local/nginx/conf/nginx.conf

sudo service nginx restart

popd
popd
popd
