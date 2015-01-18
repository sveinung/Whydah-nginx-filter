#!/bin/sh
nohup /usr/bin/java -DIAM_MODE=TEST_LOCALHOST -DIAM_CONFIG=ssologinwebapp.TEST.properties -jar SSOLoginWebApp.jar
