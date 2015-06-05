mod-spdy
========

[![Build Status](https://travis-ci.org/eousphoros/mod-spdy.svg?branch=master)](https://travis-ci.org/eousphoros/mod-spdy)

OpenSSL 1.0.2 and Apache 2.4.12 port for mod-ssl with npn support, TLS_FALLBACK_SCSV and mod-spdy. If you are looking for 2.4.7 (The version that is currently shipping with Ubuntu LTS, use the 2.4.7 branch)

Status: Functional. Cleanup pending.

apache2-2.4.12 + mod-ssl(npn,1.0.2) + mod-spdy


Quick Start
===========
```sh
$ sudo apt-get -y install git g++ apache2 libapr1-dev libaprutil1-dev patch binutils make devscripts libpcre3-dev
$ git clone https://github.com/eousphoros/mod-spdy.git
$ cd mod-spdy/src
$ ./build_modssl_with_npn.sh
$ chmod +x ./build/gyp_chromium
$ make BUILDTYPE=Release
```
> If everything is successful you should have mod-spdy/src/out/Release/libmod_spdy.so and mod-spdy/src/mod_ssl.so which can be installed into your apache2.4 modules directory.

Installation
------------
Following steps should work fine for Debian (execute as root):

- copy to apache modules (it's not wise to replace system mod_ssl.so so name it mod_ssl_npn.so)
```sh
cp out/Release/libmod_spdy.so /usr/lib/apache2/modules/mod_spdy.so
cp mod_ssl.so /usr/lib/apache2/modules/mod_ssl_npn.so
```
- prepare configuration for mod_spdy
```sh
echo "LoadModule spdy_module /usr/lib/apache2/modules/mod_spdy.so" | sudo tee /etc/apache2/mods-available/spdy.load
echo "SpdyEnabled on" | tee /etc/apache2/mods-available/spdy.conf
```
- prepare configuration for npn-enabled ssl (suppose you have regular mod_ssl installed)
```sh
sed s,mod_ssl.so,mod_ssl_npn.so,g /etc/apache2/mods-available/ssl.load > /etc/apache2/mods-available/ssl_npn.load
ln -s /etc/apache2/mods-available/ssl.conf /etc/apache2/mods-available/ssl_npn.conf
```
- unload original mod_ssl and load mod_spdy and mod_spdy_npn
```sh
a2dismod ssl
a2enmod ssl_npn
a2enmod spdy
```
- restart apache, check status and error log
```sh
service apache2 restart
service apache2 status
tail /var/log/apache2/error.log
```

Live Demo
=========

https://blck.io

Original Source
===============

https://code.google.com/p/mod-spdy/
