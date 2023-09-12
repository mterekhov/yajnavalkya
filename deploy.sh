#!/bin/sh

echo "Stoping yajnavalkya service"
sudo service yajnavalkya stop

echo "deploying rc.d script"
sudo cp yajnavalkya /usr/local/etc/rc.d/

echo "deploying yajnavalkya binary"
sudo cp build/debug/yajnavalkya /usr/local/bin/

echo "starting yajnavalkya service"
sudo service yajnavalkya start
