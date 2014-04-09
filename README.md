mod-spdy
========

[![Build Status](https://travis-ci.org/eousphoros/mod-spdy.svg?branch=master)](https://travis-ci.org/eousphoros/mod-spdy)

OpenSSL 1.0.1(g) and Apache 2.4.7 port for mod-ssl with npn support and mod-spdy. Tested under Ubuntu 14.04 by eousphoros, tested under Ubuntu 13.10 by René Kijewski. Travic-CI testing by Julian K.

Status: Functional. Cleanup pending.

apache2-2.4.7-1ubuntu1 + mod-ssl(npn,1.0.1g) + mod-spdy
https://www.ssllabs.com/ssltest/analyze.html?d=blck.io NPN:  spdy/3.1 spdy/3 spdy/2 http/1.1 x-mod-spdy/0.9.4.1-b1cbd2b


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
> If everything is successful you should have mod-spdy/src/out/Release/libmod_spdy.so and /mod-spdy/src/mod_ssl.so which can be installed into your apache2.4 modules directory.

Live Demo
=========

https://blck.io

Original Source
===============

https://code.google.com/p/mod-spdy/
