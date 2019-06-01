// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2010-2019 AriA developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x3c89c86f286a6f395c01e0b939c604d80eba4910fbb6bda0ad3906779c37d456")); //Starting Point
    //(100, uint256("0x000001703f50a5e350edcf11d86d8020d0fa4b8ab3ddb7157442d21513c19578")); //Change Reward
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    0, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    0        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    0,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    0,
    0,
    0};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x7c;
        pchMessageStart[1] = 0x70;
        pchMessageStart[2] = 0x0a;
        pchMessageStart[3] = 0x72;
        vAlertPubKey = ParseHex("045ecefd3b1518badf895a35783acb866cc2b105f1febaedf35bfec91c773da1e5f63a4a0cbdd7dad6d1dab924fb4261823244a8b6678fa67a825222246d3d77e6");
        nDefaultPort = 45000;
        bnProofOfWorkLimit = ~uint256(0) >> 20; //20 AriA starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 30; // AriA: 0.5 minutes
        nTargetSpacingSlowLaunch = 10 * 60;  // AriA: 10 minutes (Slow launch - Block 288)
        nTargetSpacing = 1 * 60;  // AriA: 60 Seconds
        nMaturity = 30;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 500000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 100000;
        nModifierUpdateBlock = 999999999;
        nZerocoinStartHeight = 999999999;
        nAccumulatorStartHeight = 1;
        nZerocoinStartTime = 1548492829; // [notused]
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = ~1; //Start enforcing the invalid UTXO's
        nBlockZerocoinV2 = 999999999; //The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1522454400; //!> Sporks signed after Sunday, February 3, 2019 12:00:00 AM Kyoto, Japan Times must use the new spork key
        nRejectOldSporkKey = 1546849804; //!> Fully reject old spork key after Moday, January 7, 2019 5:30:00 PM Kyoto, Japan Times

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
            genesis.nTime = 1527974340
            genesis.nNonce = 236551
            genesis.nVersion = 1
            genesis.GetHash = 000003f46e57c04e31374710022a6685abea12cb99960dfe96eaaa430edc670f
            genesis.hashMerkleRoot = 90d274dca16659997d8f5fc9644127d260087d4a39ca61ba9b3334482aa8c55d

         */
        const char* pszTimestamp = "2/15/2019 - MuBdI replace AriA Platform for zEtA";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0445cac0cfb8a925918bfd9303ca039b159963f3982a0769f8ce2d845d61774c5087a6c3185e6ccc4b5d4394378fff2579d6c6f2e45662e075f8d8c1f116bfd8fb") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1558200154;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 550989;
 
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x3c89c86f286a6f395c01e0b939c604d80eba4910fbb6bda0ad3906779c37d456"));
        assert(genesis.hashMerkleRoot == uint256("0x690e218bc75ec34b1bc02d0ae78927358b9fc3ba6e8fc7a54117e4bd10ce9306"));

    	vSeeds.push_back(CDNSSeedData("144.217.89.214", "144.217.89.214"));             // central_nodes
	vSeeds.push_back(CDNSSeedData("142.93.89.52", "142.93.89.52"));             // seed1

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 122); // r
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 66); // T
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 135); // w
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0xb5)(0x49)(0x70)(0xbc).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0xd9)(0x06)(0x18)(0x5f).convert_to_container<std::vector<unsigned char> >();
        // 	BIP used MuBdI Consensus
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0xbb)(0x45)(0xa1)(0x82).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "041dc87cc1e89ee19a8d1e45ee7140eaf82024bd3d184daac0a285b83cb1bd79fca8b7416998bb22ca92f92ef74741834117aa0c4c4974196f9c718e4a870daf5b";
        strSporkKeyOld = "0459eede7626441f7802af2736cb3a4aeb3e1f95070cde39d068a4f16525ee8fdd3c075f29f9e115aeb91952239194aa6ac19765574fed8a0d7f174f2b450e9630";
        strObfuscationPoolDummyAddress = "r9jvZy5kSYrpKb1KcpSaXcRWbZWEdMKs6m";
        nStartMasternodePayments = 1555839176 + 1500; //Fri, 26 April 2019 20:36:16 GMT

        /** Zerocoin */
        zerocoinModulus = "0xc95577b6dce0049b0a20c779af38079355abadde1a1d80c353f6cb697a7ae5a087bad39caa5798478551d0f9d91e6267716506f32412de1d19d17588765eb9502b85c6a18abdb05791cfd8b734e960281193705eeece210920cc922b3af3ceb178bf12c22eb565d5767fbf19545639be8953c2c38ffad41f3371e4aac750ac2d7bd614b3faabb453081d5d88fdbb803657a980bc93707e4b14233a2358c97763bf28f7c933206071477e8b371f229bc9ce7d6ef0ed7163aa5dfe13bc15f7816348b328fa2c1e69d5c88f7b94cee7829d56d1842d77d7bb8692e9fc7b7db059836500de8d57eb43c345feb58671503b932829112941367996b03871300f25efb5";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * ZCENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
	nZerocoinRequiredStakeDepth = 300; //The required confirmations for a zar to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x65;
        pchMessageStart[1] = 0x2a;
        pchMessageStart[2] = 0x24;
        pchMessageStart[3] = 0x08;
        vAlertPubKey = ParseHex("04d657eab6c67f2d7aeb234d142e67ddf0da26a678aa3e78f7172f6b575ac5af0c88a90e31338ce570e57c048d9a76be205e186aa209880b00c93537f8a59f0488");
        nDefaultPort = 25121;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // AriA: 1 day
        nTargetSpacing = 1 * 60;  // AriA: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 1 * COIN;
        nZerocoinStartHeight = 999999999;
        nZerocoinStartTime = 1524711188;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 902850; //Start enforcing the invalid UTXO's
	    nBlockZerocoinV2 = 999999999; //The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1522454400; //!> Sporks signed after Sunday, February 3, 2019 12:00:00 AM Kyoto, Japan Times must use the new spork key
        nRejectOldSporkKey = 1546849804; //!> Fully reject old spork key after Moday, January 7, 2019 5:30:00 PM Kyoto, Japan Times

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1535038683;
        genesis.nNonce = 79855;

	hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0xc43676aa953cfe8167507016eaa91575b1231a821a8ae1b5d0a22dd10823622f"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // vSeeds.push_back(CDNSSeedData("45.76.61.28", "207.148.0.129"));         // Single node address
        // vSeeds.push_back(CDNSSeedData("209.250.240.94", "45.77.239.30"));       // Single node address
        // vSeeds.push_back(CDNSSeedData("45.77.176.204", "45.76.226.204"));       // Single node address


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 23); // Testnet AriA addresses start with 'a'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 61);  // Testnet AriA script addresses start with 'r'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 41);     // Testnet private keys start with 'h' (AriA defaults)
        // Testnet AriA BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x24)(0x94)(0x80)(0x93).convert_to_container<std::vector<unsigned char> >();
        // Testnet AriA BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x24)(0x94)(0xfa)(0xca).convert_to_container<std::vector<unsigned char> >();
        // Testnet AriA BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04c5329e76ac673b39c9998546497fbc0dc03f3614b8452b29481ff3f6641a62a53b4aec8f0fe4101cc667864e3ce613ee54e823d491d1d8cc135a48256add7812";
        strSporkKeyOld = "04f01b5a9a36c86e2739e20a35c32196bba6ecf49a5d566e50ae1d6db2c7ed50a05ae7b35f81aafa6f837993fc4780d31c96243831288ea167e0bb48157e8e4154";
        strObfuscationPoolDummyAddress = "xp87cG8UEQgzs1Bk67Yk884C7pnQfAeo7q";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xc7;
        pchMessageStart[1] = 0xa9;
        pchMessageStart[2] = 0xbd;
        pchMessageStart[3] = 0x1b;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // AriA: 1 day
        nTargetSpacing = 1 * 60;        // AriA: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1535040987;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 232084;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 17808;
        //assert(hashGenesisBlock == uint256("0x000008415bdca132b70cf161ecc548e5d0150fd6634a381ee2e99bb8bb77dbb3"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 2202;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
