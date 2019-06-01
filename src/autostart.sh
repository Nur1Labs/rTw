#!/bin/sh
# Copyright (c) 2013-2019 The AriA Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
./rtw-cli stop
./rtwd -daemon -pid=rtw.pid
