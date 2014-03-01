Experimental Branch for spdy/3.1

mod-spdy
========

OpenSSL 1.0.1(f) and Apache 2.4.7 port for mod-ssl with npn support and mod-spdy. Tested under Ubuntu 14.04.

Status: Functional. Cleanup pending.

apache2-2.4.7-1ubuntu1 + mod-ssl(npn,1.0.1f) + mod-spdy
https://www.ssllabs.com/ssltest/analyze.html?d=blck.io NPN:  spdy/3.1 spdy/3 spdy/2 http/1.1 x-mod-spdy/0.9.4.1-b1cbd2b


Required Packages
=================

$ sudo apt-get install g++ apache2 libapr1-dev libaprutil1-dev patch binutils make devscripts

Live Demo
=========

https://blck.io

Original Source
===============

https://code.google.com/p/mod-spdy/
