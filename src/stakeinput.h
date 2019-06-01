// Copyright (c) 2010-2019 AriA developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ARIA_STAKEINPUT_H
#define ARIA_STAKEINPUT_H

class CKeyStore;
class CWallet;
class CWalletTx;

class CStakeInput
{
protected:
    CBlockIndex* pindexFrom;

public:
    virtual ~CStakeInput(){};
    virtual CBlockIndex* GetIndexFrom() = 0;
    virtual bool CreateTxIn(CWallet* pwallet, CTxIn& txIn, uint256 hashTxOut = 0) = 0;
    virtual bool GetTxFrom(CTransaction& tx) = 0;
    virtual CAmount GetValue() = 0;
    virtual bool CreateTxOuts(CWallet* pwallet, vector<CTxOut>& vout) = 0;
    virtual bool GetModifier(uint64_t& nStakeModifier) = 0;
    virtual bool IsZAriA() = 0;
    virtual CDataStream GetUniqueness() = 0;
};


// zAriAStake can take two forms
// 1) the stake candidate, which is a zcmint that is attempted to be staked
// 2) a staked zAriA, which is a zcspend that has successfully staked
class CZAriAStake : public CStakeInput
{
private:
    uint32_t nChecksum;
    CZerocoinMint mint;
    bool fMint;
    libzerocoin::CoinDenomination denom;
    CBigNum bnSerial;


public:
    explicit CZAriAStake(CZerocoinMint mint)
    {
        this->mint = mint;
        this->denom = mint.GetDenomination();
        this->bnSerial = mint.GetSerialNumber();
        this->pindexFrom = nullptr;
        fMint = true;
    }

    explicit CZAriAStake(libzerocoin::CoinSpend spend)
    {
        this->nChecksum = spend.getAccumulatorChecksum();
        this->denom = spend.getDenomination();
        this->bnSerial = spend.getCoinSerialNumber();
        this->pindexFrom = nullptr;
        fMint = false;
    }

    CBlockIndex* GetIndexFrom() override;
    bool GetTxFrom(CTransaction& tx) override;
    CAmount GetValue() override;
    bool GetModifier(uint64_t& nStakeModifier) override;
    CDataStream GetUniqueness() override;
    bool CreateTxIn(CWallet* pwallet, CTxIn& txIn, uint256 hashTxOut = 0) override;
    bool CreateTxOuts(CWallet* pwallet, vector<CTxOut>& vout) override;
    bool MarkSpent(CWallet* pwallet);
    bool IsZAriA() override { return true; }
    int GetChecksumHeightFromMint();
    int GetChecksumHeightFromSpend();
    uint32_t GetChecksum();
};

class CAriAStake : public CStakeInput
{
private:
    CTransaction txFrom;
    unsigned int nPosition;
public:
    CAriAStake()
    {
        this->pindexFrom = nullptr;
    }

    bool SetInput(CTransaction txPrev, unsigned int n);

    CBlockIndex* GetIndexFrom() override;
    bool GetTxFrom(CTransaction& tx) override;
    CAmount GetValue() override;
    bool GetModifier(uint64_t& nStakeModifier) override;
    CDataStream GetUniqueness() override;
    bool CreateTxIn(CWallet* pwallet, CTxIn& txIn, uint256 hashTxOut = 0) override;
    bool CreateTxOuts(CWallet* pwallet, vector<CTxOut>& vout) override;
    bool IsZAriA() override { return false; }
};


#endif //ARIA_STAKEINPUT_H
