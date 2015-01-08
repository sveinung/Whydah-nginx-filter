set -e

pushd /vagrant/provision

sudo apt-get install make
sudo apt-get install openjdk-7-jre-headless

sudo cp start-user-admin-service.sh /home/vagrant
sudo cp useradminservice.TEST.properties /home/vagrant

popd
