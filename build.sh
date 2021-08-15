#! /usr/bin/sh

mkdir dist -p

g++ $@ -pthread -Ivendor/include/ -o dist/bot src/*.cc -Lvendor/lib/curlpp -lssl -lcrypto -lcurl -lcurlpp  -Wl,-rpath='$ORIGIN'
cp vendor/lib/**/*.so* dist