set -e

pushd /vagrant/provision

sudo apt-get install make

sudo cp nginx /etc/init.d/
sudo chmod +x /etc/init.d/nginx

sudo cp /vagrant/provision/nginx.conf /usr/local/nginx/conf/nginx.conf

sudo service nginx restart

popd
