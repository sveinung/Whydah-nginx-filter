#!/bin/sh

#export IAM_MODE=DEV
export IAM_MODE=TEST
#export IAM_MODE=PROD

A=UserAdminWebApp
V=LATEST
JARFILE=$A.jar

pkill -f $A

nohup java -jar -DIAM_CONFIG=useradminwebapp.TEST.properties $JARFILE &

tail -f nohup.out
