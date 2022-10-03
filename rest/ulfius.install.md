## Install on Linux:
https://github.com/babelouest/ulfius/blob/master/INSTALL.md

For Debian 11 bullseye:

```
$sudo apt install -y libmicrohttpd-dev libjansson-dev libcurl4-gnutls-dev libgnutls28-dev libgcrypt20-dev libsystemd-dev
$sudo apt install libulfius-dev uwsc
$sudo apt install libapr1 libaprutil1 libaprutil1-dev
$make -f Linux.Makefile

```


## Install on Mac:
brew install gnutls
brew install libmicrohttpd

编译 ulfius， 自动下载依赖:
$ git clone https://github.com/babelouest/ulfius.git
$ cd ulfius
$ mkdir build
$ cd build
$ cmake -DWITH_WEBSOCKET=off ..

编译 orcania 之后， 拷贝到 /usr/local/lib 目录
$ cd orcania-src/src
$ make
$ make install 
$ cp orcania-src/src/liborcania.so* /usr/local/lib
$ cd ..

编译 yder , install:
$ cd yder/src 
$ make Y_DISABLE_JOURNALD=1
$ sudo make install
$ cp libyder.so* /usr/local/lib

继续编译 ulfius:
$ cd ../
$ make
$ make install # as admin


## Install on Windows

On windows, you must first install all dependencies (except for libsystemd which isn't available on Windows).
At least libmicrohttpd is required.

Then you need to disable these options:  -DWITH_JOURNALD=off -DWITH_WEBSOCKET=off

https://github.com/babelouest/ulfius/issues/23
https://github.com/babelouest/ulfius/issues/111
