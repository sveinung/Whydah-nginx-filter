set -e

DIR=/home/vagrant

pushd /vagrant/provision

sudo apt-get update
sudo apt-get install -y make curl openjdk-7-jre-headless

sudo cp hosts /etc/hosts

sudo mkdir -p /usr/local/nginx/ssl/
sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout /usr/local/nginx/ssl/nginx.key -out /usr/local/nginx/ssl/nginx.crt -config openssl.config -batch

mkdir -p $DIR/useradminservice
pushd useradminservice
sudo cp start-user-admin-service.sh $DIR/useradminservice
sudo cp useradminservice.TEST.properties $DIR/useradminservice
chown -R vagrant:vagrant $DIR/useradminservice
popd

mkdir -p $DIR/securitytokenservice
pushd securitytokenservice
sudo cp update-security-token-service.sh $DIR/securitytokenservice
sudo cp start-security-token-service.sh $DIR/securitytokenservice
sudo cp securitytokenservice.TEST.properties $DIR/securitytokenservice
sudo cp t_whydah_user.token $DIR/securitytokenservice
chown -R vagrant:vagrant $DIR/securitytokenservice
popd

mkdir -p $DIR/sso-login-webapp
pushd sso-login-webapp
sudo cp update-service.sh $DIR/sso-login-webapp
sudo cp start-service.sh $DIR/sso-login-webapp
sudo cp ssologinwebapp.TEST.properties $DIR/sso-login-webapp
chown -R vagrant:vagrant $DIR/sso-login-webapp
popd

mkdir -p $DIR/user-admin-webapp
pushd user-admin-webapp
sudo cp update-service.sh $DIR/user-admin-webapp
sudo cp start-service.sh $DIR/user-admin-webapp
sudo cp useradminwebapp.TEST.properties $DIR/user-admin-webapp
chown -R vagrant:vagrant $DIR/user-admin-webapp
popd

mkdir -p $DIR/user-admin-webapp
pushd user-admin-webapp
sudo cp update-service.sh $DIR/user-admin-webapp
sudo cp start-service.sh $DIR/user-admin-webapp
sudo cp useradminwebapp.TEST.properties $DIR/user-admin-webapp
chown -R vagrant:vagrant $DIR/user-admin-webapp
popd

mkdir -p $DIR/user-identity-backend
pushd user-identity-backend
sudo cp update-service.sh $DIR/user-identity-backend
sudo cp start-service.sh $DIR/user-identity-backend
sudo cp useridentitybackend.TEST.properties $DIR/user-identity-backend
sudo cp -r testdata/ $DIR/user-identity-backend/testdata
chown -R vagrant:vagrant $DIR/user-identity-backend
popd

popd
