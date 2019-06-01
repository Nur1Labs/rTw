# Copyright (c) 2010 Satoshi Nakamoto
# Copyright (c) 2010-2019 AriA developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS) $(ERROR_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) $(AM_CPPFLAGS) $(CPPFLAGS) -D__STDC_LIMIT_MACROS"
endif

ARIA_CONFIG_INCLUDES=-I$(builddir)/config
ARIA_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

ARIA_INCLUDES += -I$(srcdir)/secp256k1/include
ARIA_INCLUDES += $(UNIVALUE_CFLAGS)

LIBARIA_SERVER=libaria_server.a
LIBARIA_COMMON=libaria_common.a
LIBARIA_CLI=libaria_cli.a
LIBARIA_UTIL=libaria_util.a
LIBARIA_CRYPTO=crypto/libaria_crypto.a
LIBARIA_ZEROCOIN=libzerocoin/libaria_zerocoin.a
LIBARIAQT=qt/libariaqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

if ENABLE_ZMQ
LIBARIA_ZMQ=libaria_zmq.a
endif
if BUILD_ARIA_LIBS
LIBARIACONSENSUS=libariaconsensus.la
endif
if ENABLE_WALLET
LIBARIA_WALLET=libaria_wallet.a
endif

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBARIA_CRYPTO) \
  $(LIBARIA_UTIL) \
  $(LIBARIA_COMMON) \
  $(LIBARIA_ZEROCOIN) \
  $(LIBARIA_SERVER) \
  $(LIBARIA_CLI) \
  $(LIBARIA_WALLET) \
  $(LIBARIA_ZMQ)

lib_LTLIBRARIES = $(LIBARIACONSENSUS)

bin_PROGRAMS =
noinst_PROGRAMS =
TESTS =

if BUILD_ARIAD
  bin_PROGRAMS += rtwd
endif

if BUILD_ARIA_UTILS
  bin_PROGRAMS += rtw-cli aria-tx
endif

.PHONY: FORCE
# aria core #
ARIA_CORE_H = \
  activemasternode.h \
  accumulators.h \
  accumulatormap.h \
  addrman.h \
  alert.h \
  allocators.h \
  amount.h \
  base58.h \
  bip38.h \
  bloom.h \
  blocksignature.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
  compat/sanity.h \
  compressor.h \
  primitives/block.h \
  primitives/transaction.h \
  primitives/zerocoin.h \
  core_io.h \
  crypter.h \
  denomination_functions.h \
  obfuscation.h \
  obfuscation-relay.h \
  db.h \
  eccryptoverify.h \
  ecwrapper.h \
  hash.h \
  httprpc.h \
  httpserver.h \
  init.h \
  kernel.h \
  swifttx.h \
  key.h \
  keystore.h \
  leveldbwrapper.h \
  limitedmap.h \
  main.h \
  masternode.h \
  masternode-payments.h \
  masternode-budget.h \
  masternode-sync.h \
  masternodeman.h \
  masternodeconfig.h \
  merkleblock.h \
  miner.h \
  mruset.h \
  netbase.h \
  net.h \
  noui.h \
  pow.h \
  protocol.h \
  pubkey.h \
  random.h \
  reverselock.h \
  reverse_iterate.h \
  rpcclient.h \
  rpcprotocol.h \
  rpcserver.h \
  scheduler.h \
  script/interpreter.h \
  script/script.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/script_error.h \
  serialize.h \
  spork.h \
  sporkdb.h \
  stakeinput.h \
  streams.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilstrencodings.h \
  utilmoneystr.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  wallet.h \
  wallet_ismine.h \
  walletdb.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h \
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh "$(abs_top_builddir)/src/obj/build.h" \
	  "$(abs_top_srcdir)"
libaria_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between ariad and aria-qt
libaria_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(ARIA_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libaria_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaria_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  blocksignature.cpp \
  chain.cpp \
  checkpoints.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  pow.cpp \
  rest.cpp \
  rpcblockchain.cpp \
  rpcmasternode.cpp \
  rpcmasternode-budget.cpp \
  rpcmining.cpp \
  rpcmisc.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  rpcserver.cpp \
  script/sigcache.cpp \
  sporkdb.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  validationinterface.cpp \
  $(ARIA_CORE_H)

if ENABLE_ZMQ
libaria_zmq_a_CPPFLAGS = $(ARIA_INCLUDES) $(ZMQ_CFLAGS)
libaria_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaria_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif

# wallet: shared between ariad and aria-qt, but only linked
# when wallet enabled
libaria_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(ARIA_INCLUDES)
libaria_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaria_wallet_a_SOURCES = \
  activemasternode.cpp \
  bip38.cpp \
  denomination_functions.cpp \
  obfuscation.cpp \
  obfuscation-relay.cpp \
  db.cpp \
  crypter.cpp \
  swifttx.cpp \
  masternode.cpp \
  masternode-budget.cpp \
  masternode-payments.cpp \
  masternode-sync.cpp \
  masternodeconfig.cpp \
  masternodeman.cpp \
  rpcdump.cpp \
  primitives/zerocoin.cpp \
  rpcwallet.cpp \
  kernel.cpp \
  wallet.cpp \
  wallet_ismine.cpp \
  walletdb.cpp \
  stakeinput.cpp \
  $(ARIA_CORE_H)

# crypto primitives library
crypto_libaria_crypto_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libaria_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libaria_crypto_a_SOURCES = \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/hmac_sha256.cpp \
  crypto/rfc6979_hmac_sha256.cpp \
  crypto/hmac_sha512.cpp \
  crypto/scrypt.cpp \
  crypto/ripemd160.cpp \
  crypto/sph_md_helper.c \
  crypto/sph_sha2big.c \
  crypto/aes_helper.c \
  crypto/blake.c \
  crypto/bmw.c \
  crypto/groestl.c \
  crypto/jh.c \
  crypto/luffa.c \
  crypto/keccak.c \
  crypto/skein.c \
  crypto/cubehash.c \
  crypto/shavite.c \
  crypto/shabal.c \
  crypto/simd.c \
  crypto/echo.c \
  crypto/whirlpool.c \
  crypto/haval.c \
  crypto/hamsi.c \
  crypto/fugue.c \
  crypto/sha2.c \
  crypto/common.h \
  crypto/sha256.h \
  crypto/sha512.h \
  crypto/hmac_sha256.h \
  crypto/rfc6979_hmac_sha256.h \
  crypto/hmac_sha512.h \
  crypto/scrypt.h \
  crypto/sha1.h \
  crypto/ripemd160.h \
  crypto/sph_blake.h \
  crypto/sph_bmw.h \
  crypto/sph_groestl.h \
  crypto/sph_jh.h \
  crypto/sph_keccak.h \
  crypto/sph_luffa.h \
  crypto/sph_shavite.h \
  crypto/sph_hamsi.h \
  crypto/sph_cubehash.h \
  crypto/sph_skein.h \
  crypto/sph_simd.h \
  crypto/sph_fugue.h \
  crypto/sph_echo.h \
  crypto/sph_shabal.h \
  crypto/sph_whirlpool.h \
  crypto/sph_sha2.h \
  crypto/sph_haval.h \
  crypto/sph_types.h

# libzerocoin library
libzerocoin_libaria_zerocin_a_CPPFLAGS = $(AM_CPPFLAGS)
libzerocoin_libaria_zerocin_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libzerocoin_libaria_zerocoin_a_SOURCES = \
  libzerocoin/Accumulator.h \
  libzerocoin/AccumulatorProofOfKnowledge.h \
  libzerocoin/bignum.h \
  libzerocoin/Coin.h \
  libzerocoin/CoinSpend.h \
  libzerocoin/Commitment.h \
  libzerocoin/Denominations.h \
  libzerocoin/ParamGeneration.h \
  libzerocoin/Params.h \
  libzerocoin/SerialNumberSignatureOfKnowledge.h \
  libzerocoin/SpendType.h \
  libzerocoin/ZerocoinDefines.h \
  libzerocoin/Accumulator.cpp \
  libzerocoin/AccumulatorProofOfKnowledge.cpp \
  libzerocoin/Coin.cpp \
  libzerocoin/Denominations.cpp \
  libzerocoin/CoinSpend.cpp \
  libzerocoin/Commitment.cpp \
  libzerocoin/ParamGeneration.cpp \
  libzerocoin/Params.cpp \
  libzerocoin/SerialNumberSignatureOfKnowledge.cpp

# common: shared between ariad, and aria-qt and non-server tools
libaria_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(ARIA_INCLUDES)
libaria_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaria_common_a_SOURCES = \
  accumulators.cpp \
  accumulatormap.cpp \
  allocators.cpp \
  amount.cpp \
  base58.cpp \
  bip38.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  primitives/zerocoin.cpp \
  core_read.cpp \
  core_write.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  pubkey.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/sign.cpp \
  script/standard.cpp \
  script/script_error.cpp \
  spork.cpp \
  sporkdb.cpp \
  $(ARIA_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libaria_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(ARIA_INCLUDES)
libaria_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaria_util_a_SOURCES = \
  allocators.cpp \
  compat/strnlen.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  random.cpp \
  rpcprotocol.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilstrencodings.cpp \
  utilmoneystr.cpp \
  utiltime.cpp \
  $(ARIA_CORE_H)

if GLIBC_BACK_COMPAT
libaria_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between aria-cli and aria-qt
libaria_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(ARIA_INCLUDES)
libaria_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libaria_cli_a_SOURCES = \
  rpcclient.cpp \
  $(ARIA_CORE_H)

nodist_libaria_util_a_SOURCES = $(srcdir)/obj/build.h
#

# ariad binary #
rtwd_SOURCES = ariad.cpp
rtwd_CPPFLAGS = $(AM_CPPFLAGS) $(ARIA_INCLUDES)
rtwd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
rtwd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
rtwd_SOURCES += ariad-res.rc
endif
rtwd_LDADD = \
  $(LIBARIA_SERVER) \
  $(LIBARIA_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBARIA_ZEROCOIN) \
  $(LIBARIA_UTIL) \
  $(LIBARIA_WALLET) \
  $(LIBARIA_ZMQ) \
  $(LIBARIA_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

rtwd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)

# aria-cli binary #
rtw_cli_SOURCES = aria-cli.cpp
rtw_cli_CPPFLAGS = $(AM_CPPFLAGS) $(ARIA_INCLUDES) $(EVENT_CFLAGS)
rtw_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
rtw_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
rtw_cli_SOURCES += aria-cli-res.rc
endif

rtw_cli_LDADD = \
  $(LIBARIA_CLI) \
  $(LIBUNIVALUE) \
  $(LIBARIA_UTIL) \
  $(LIBARIA_CRYPTO)

rtw_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# aria-tx binary #
aria_tx_SOURCES = aria-tx.cpp
aria_tx_CPPFLAGS = $(AM_CPPFLAGS) $(ARIA_INCLUDES)
aria_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
aria_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
aria_tx_SOURCES += aria-tx-res.rc
endif

aria_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBARIA_COMMON) \
  $(LIBARIA_ZEROCOIN) \
  $(LIBARIA_UTIL) \
  $(LIBARIA_CRYPTO) \
  $(LIBSECP256K1)

aria_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# ariaconsensus library #
if BUILD_ARIA_LIBS
include_HEADERS = script/ariaconsensus.h
libariaconsensus_la_SOURCES = \
  allocators.cpp \
  primitives/transaction.cpp \
  crypto/hmac_sha512.cpp \
  crypto/scrypt.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/ripemd160.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  pubkey.cpp \
  script/script.cpp \
  script/interpreter.cpp \
  script/ariaconsensus.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libariaconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libariaconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libariaconsensus_la_LIBADD = $(CRYPTO_LIBS) $(BOOST_LIBS)
libariaconsensus_la_CPPFLAGS = $(CRYPTO_CFLAGS) -I$(builddir)/obj -DBUILD_ARIA_INTERNAL
if USE_LIBSECP256K1
libariaconsensus_la_LIBADD += secp256k1/libsecp256k1.la
endif
endif

CLEANFILES = $(EXTRA_LIBRARIES)
CLEANFILES += leveldb/libleveldb.a leveldb/libmemenv.a
CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno
CLEANFILES += obj/build.h


EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	rm -f leveldb/*/*.gcno leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@


%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
