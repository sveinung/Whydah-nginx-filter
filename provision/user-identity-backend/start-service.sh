#!/bin/sh
nohup /usr/bin/java -DIAM_MODE=PROD -DIAM_CONFIG=useridentitybackend.TEST.properties -jar UserIdentityBackend.jar &
