set -e

DIR=/home/vagrant

pushd /vagrant/provision

sudo apt-get install make
sudo apt-get install openjdk-7-jre-headless

mkdir -p $DIR/useradminservice
chown vagrant:vagrant $DIR/useradminservice
sudo cp start-user-admin-service.sh $DIR/useradminservice
sudo cp useradminservice.TEST.properties $DIR/useradminservice

mkdir -p $DIR/securitytokenservice
chown vagrant:vagrant $DIR/securitytokenservice
sudo cp update-security-token-service.sh $DIR/securitytokenservice
sudo cp start-security-token-service.sh $DIR/securitytokenservice
sudo cp securitytokenservice.TEST.properties $DIR/securitytokenservice

popd
