#!/bin/sh
nohup /usr/bin/java -DIAM_MODE=TEST_LOCALHOST -Dhazelcast.config=hazelcast.xml -DIAM_CONFIG=securitytokenservice.TEST.properties -jar SecurityTokenService.jar
