#! /usr/bin/sh

echo -- Building!

mkdir dist -p

g++ $@ -pthread -Ivendor/include/ -o dist/bot src/*.cc -Lvendor/lib/curlpp -lssl -lcrypto -lcurl -lcurlpp  -Wl,-rpath='$ORIGIN'

if test -e dist; then
    cp vendor/lib/**/*.so* dist
fi

echo -- Done!