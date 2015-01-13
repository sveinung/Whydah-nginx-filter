Whydah-nginx-filter
===================

Provides authentication in Nginx by integrating with [Whydah](https://wiki.altrancloud.com/display/whydah/Whydah+Home)

Useful when you need Whydah authentication in front of third-party applications.

Sample config:
```nginx
http {
    server {
        listen       80;
        server_name  localhost;

        location / {
            root   html;
            index  index.html index.htm;

            whydah on;
            whydah_sso_login_webapp_url https://demo.getwhydah.com/sso/login;
            whydah_roles roleA roleB roleC;
        }
    }
}
```
