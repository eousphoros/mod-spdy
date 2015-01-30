mod-spdy
========

[![Build Status](https://travis-ci.org/eousphoros/mod-spdy.svg?branch=master)](https://travis-ci.org/eousphoros/mod-spdy)

OpenSSL 1.0.1(j) and Apache 2.4.10 port for mod-ssl with npn support, TLS_FALLBACK_SCSV and mod-spdy. If you are looking for 2.4.7 (The version that is currently shipping with Ubuntu LTS, use the 2.4.7 branch)

Status: Functional. Cleanup pending.

apache2-2.4.10 + mod-ssl(npn,1.0.1j) + mod-spdy


Quick Start
===========
```sh
$ sudo apt-get -y install git g++ apache2 libapr1-dev libaprutil1-dev patch binutils make devscripts
$ git clone https://github.com/eousphoros/mod-spdy.git
$ cd mod-spdy/src
$ ./build_modssl_with_npn.sh
$ chmod +x ./build/gyp_chromium
$ make BUILDTYPE=Release
````
> If everything is successful you should have mod-spdy/src/out/Release/libmod_spdy.so and mod-spdy/src/mod_ssl.so which can be installed into your apache2.4 modules directory.

Live Demo
=========

https://blck.io

Original Source
===============

https://code.google.com/p/mod-spdy/
