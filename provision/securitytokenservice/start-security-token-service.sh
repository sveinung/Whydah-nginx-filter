#!/bin/sh
nohup /usr/bin/java -DIAM_MODE=DEV -Dhazelcast.config=hazelcast.xml -DIAM_CONFIG=securitytokenservice.TEST.properties -jar SecurityTokenService.jar
