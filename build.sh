#! /usr/bin/sh

mkdir dist -p

g++ $@ -Ivendor/include/ -o dist/bot src/*.cc -Lvendor/lib/curlpp -lcurl -lcurlpp -Wl,-rpath='$ORIGIN'
cp vendor/lib/**/*.so* dist