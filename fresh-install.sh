#!/bin/sh
# Copyright (c) 2010-2019 Nur1Labs.Ltd
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

#this for autoinstall
apt install -y build-essential libtool autotools-dev automake pkg-config libssl-dev bsdmainutils libboost-all-dev libminiupnpc-dev libzmq3-dev git wget libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libqrencode-dev
git clone https://github.com/libevent/libevent.git
cd libevent
./autogen.sh && ./configure && make && make install && cd ..

#db
wget 'http://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz'
tar -xzvf db-4.8.30.NC.tar.gz
cd db-4.8.30.NC/build_unix/
../dist/configure --enable-cxx --disable-shared --with-pic
make install && cd ../../..

#tar
cd recycle && tar -xvf plugin.tar.gz && cd src/test && tar -xvf py.tar.gz && cd .. && cd crypto && tar -xvf sph.tar.gz && cd ../../.. && chmod 755 -R recycle
cd recycle && ./aria.sh && ./configure CPPFLAGS="-I/usr/local/BerkeleyDB.4.8/include" LDFLAGS="-L/usr/local/BerkeleyDB.4.8/lib" --enable-cxx --disable-shared --with-unsupported-ssl && make && make install
