// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "db.h"
#include "init.h"
#include "bitcoinrpc.h"

using namespace json_spirit;
using namespace std;

// Return average network hashes per second based on the number blocks,
Value GetNetworkHashPS(int blocks) {

    if (pindexBest == NULL)
        return 0;

    // If lookup is out of range, search from the latest block
    if ((blocks <= 0) || (blocks > pindexBest->nHeight))
        blocks = 30;

    // Find Last PoW
    const CBlockIndex* pindexLast = GetLastBlockIndex(pindexBest, false);
    double timeDiff1 = pindexLast->GetBlockTime();
    double timeDiff2 = timeDiff1;
    double count = 0;
    double diffs = 0;

    // Counting and accumulate difficulties of PoW
    for (int i = pindexLast->nHeight; i > 1; i++) {
        if (pindexLast->pprev == NULL) {
            break;
        }
        pindexLast = pindexLast->pprev;
        if (!pindexLast->IsProofOfStake()) {
            int    nShift = (pindexLast->nBits >> 24) & 0xff;
            double dDiff = (double)0x0000ffff / (double)(pindexLast->nBits & 0x00ffffff);
            while (nShift < 29) {
                dDiff *= 256.0;
                nShift++;
            }
            while (nShift > 29) {
                dDiff /= 256.0;
                nShift--;
            }
            diffs += dDiff;
            count += 1;
            timeDiff2 = pindexLast->GetBlockTime();
            if (count >= blocks) 
               break;
        }
    }

    double timeDiff = timeDiff1 - timeDiff2;
    if (timeDiff < 1)
       return (0);

    return (boost::int64_t)((diffs * pow(2.0, 32)) / timeDiff);
}

Value getnetworkhashps(const Array& params, bool fHelp)
{
    if (fHelp || params.size() > 1)
        throw runtime_error(
            "getnetworkhashps [blocks]\n"
            "Returns the estimated network hashes per second based on the last 30 blocks.\n"
            "Pass in [blocks] to override # of blocks, -1 specifies since last difficulty change.");

    return GetNetworkHashPS(params.size() > 0 ? params[0].get_int() : 30);
}


// Key used by getwork/getblocktemplate miners.
// Allocated in InitRPCMining, free'd in ShutdownRPCMining
static CReserveKey* pMiningKey = NULL;

void InitRPCMining()
{
    if (!pwalletMain)
        return;

    // getwork/getblocktemplate mining rewards paid here:
    pMiningKey = new CReserveKey(pwalletMain);
}

void ShutdownRPCMining()
{
    if (!pMiningKey)
        return;

    delete pMiningKey; 
    pMiningKey = NULL;
}

Value getgenerate(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getgenerate\n"
            "Returns true or false.");

    if (!pMiningKey)
        return false;

    return GetBoolArg("-gen");
}


Value setgenerate(const Array& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 2)
        throw runtime_error(
            "setgenerate <generate> [genproclimit]\n"
            "<generate> is true or false to turn generation on or off.\n"
            "Generation is limited to [genproclimit] processors, -1 is unlimited.");

    bool fGenerate = true;
    if (params.size() > 0)
        fGenerate = params[0].get_bool();

    if (params.size() > 1)
    {
        int nGenProcLimit = params[1].get_int();
        mapArgs["-genproclimit"] = itostr(nGenProcLimit);
        if (nGenProcLimit == 0)
            fGenerate = false;
    }
    mapArgs["-gen"] = (fGenerate ? "1" : "0");

    assert(pwalletMain != NULL);

    GenerateBitcoins(pwalletMain, fGenerate, true);

    return Value::null;
}


Value getnostake(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getnostake\n"
            "Returns true or false.");

    return GetBoolArg("-nostake");
}


Value setnostake(const Array& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 1)
        throw runtime_error(
            "setnostake <bool>\n"
            "<bool> is true or false, to turn stake off or on.");
    bool fnostake = params[0].get_bool();
    mapArgs["-nostake"] = (fnostake ? "1" : "0");
    return Value::null;
}

Value getjackpotbet(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getjackpotbet\n"
            "Returns Jackpot Bet value.");

    return nJackpotBet;
}


Value setjackpotbet(const Array& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 1)
        throw runtime_error(
            "setjackpotbet <valuel>\n"
            "<value> is 0~100 for Jackpot Bet it will be subtracted from PoS Reward.");
    nJackpotBet = min(100, max(0, params[0].get_int()));
    return Value::null;
}

Value getjackpotluckynumber(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getjackpotluckynumber\n"
            "Returns Jackpot Lucky Number.");

    return nJackpotLucky;
}


Value setjackpotluckynumber(const Array& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 1)
        throw runtime_error(
            "setjackpotluckynumber <valuel>\n"
            "<value> is 0~15 for Jackpot Lucky Number (0 : disable)");
    nJackpotLucky = min(100, max(0, params[0].get_int()));
    return Value::null;
}

Value getcurrentjackpot(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getcurrentjackpot\n"
            "Returns the current jackpot size.");

	CBlockIndex* pblockindex = FindBlockByHeight((int)nBestHeight);
    return (boost::int64_t)GetPoWJackpot(pblockindex);
}

Value gethashespersec(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "gethashespersec\n"
            "Returns a recent hashes per second performance measurement while generating.");

    if (vHashPerSec.empty())
        return (boost::int64_t)0;

    int64 nTotal = 0;
    for (unsigned int i = 0; i < vHashPerSec.size(); i++) 
    {
        nTotal  += vHashPerSec[i];
    }
    return (boost::int64_t)(nTotal / 10);
}


Value getmininginfo(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getmininginfo\n"
            "Returns an object containing mining-related information.");

    Object obj;
    obj.push_back(Pair("blocks",             (int)nBestHeight));
    obj.push_back(Pair("currentblocksize",   (uint64)nLastBlockSize));
    obj.push_back(Pair("currentblocktx",     (uint64)nLastBlockTx));
    obj.push_back(Pair("difficulty",         (double)GetDifficulty()));
    obj.push_back(Pair("errors",             GetWarnings("statusbar")));
    obj.push_back(Pair("generate",           GetBoolArg("-gen")));
    obj.push_back(Pair("nostake",            GetBoolArg("-nostake")));
    obj.push_back(Pair("rewardPoW",          ValueFromAmount(nRewardPoWLast)));
    obj.push_back(Pair("rewardPoS",          ValueFromAmount(nRewardPoSLast)));
    obj.push_back(Pair("jackpotpotPoW",      (int)nJackpotPoW));
    obj.push_back(Pair("jackpotpotPoS",      (int)(pindexBest ? pindexBest->nPoSPot : 0)));
    obj.push_back(Pair("jackpotbet",         (int)nJackpotBet));
    obj.push_back(Pair("jackpotluckynumber", (int)nJackpotLucky));
    obj.push_back(Pair("genproclimit",       (int)GetArg("-genproclimit", -1)));
    obj.push_back(Pair("hashespersec",       gethashespersec(params, false)));
	obj.push_back(Pair("networkhashps",      getnetworkhashps(params, false)));
    obj.push_back(Pair("pooledtx",           (uint64)mempool.size()));
    obj.push_back(Pair("testnet",            fTestNet));
    return obj;
}


//
// TODO : need to return the last PoS or PoW information with PoS marking from CBlockIndex
//

Value getworkex(const Array& params, bool fHelp)
{
    if (fHelp || params.size() > 2)
        throw runtime_error(
            "getworkex [data, coinbase]\n"
            "If [data, coinbase] is not specified, returns extended work data.\n"
        );

    if (vNodes.empty())
        throw JSONRPCError(RPC_CLIENT_NOT_CONNECTED, "JackpotCoin is not connected!");

    if (IsInitialBlockDownload())
        throw JSONRPCError(RPC_CLIENT_IN_INITIAL_DOWNLOAD, "JackpotCoin is downloading blocks...");

    typedef map<uint256, pair<CBlock*, CScript> > mapNewBlock_t;
    static mapNewBlock_t mapNewBlock;    // FIXME: thread safety
    static vector<CBlock*> vBlock;
    static CReserveKey reservekey(pwalletMain);

    if (params.size() == 0)
    {
        // Update block
        static unsigned int nTransactionsUpdatedLast;
        static CBlockIndex* pindexPrev;
        static int64 nStart;
        static CBlock* pblock;
        if (pindexPrev != pindexBest ||
            (nTransactionsUpdated != nTransactionsUpdatedLast && GetTime() - nStart > 60))
        {
            if (pindexPrev != pindexBest)
            {
                // Deallocate old blocks since they're obsolete now
                mapNewBlock.clear();
                BOOST_FOREACH(CBlock* pblock, vBlock)
                    delete pblock;
                vBlock.clear();
            }

            // Clear pindexPrev so future getworks make a new block, despite any failures from here on
            pindexPrev = NULL;

            // Store the pindexBest used before CreateNewBlock, to avoid races
            nTransactionsUpdatedLast = nTransactionsUpdated;
            CBlockIndex* pindexPrevNew = pindexBest;
            nStart = GetTime();

            // Create new block
            CBlock *pblock = new CBlock();
            if (!pblock->CreateWithKey(pwalletMain, *pMiningKey, false))
                throw JSONRPCError(RPC_MISC_ERROR, "Cannot create a block");
            vBlock.push_back(pblock);

            // Need to update only after we know CreateNewBlock succeeded
            pindexPrev = pindexPrevNew;
        }

        // Update nTime
        pblock->UpdateTime(pindexPrev);
        pblock->nNonce = 0;

        // Update nExtraNonce
        static unsigned int nExtraNonce = 0;
        IncrementExtraNonce(pblock, pindexPrev, nExtraNonce);

        // Save
        mapNewBlock[pblock->hashMerkleRoot] = make_pair(pblock, pblock->vtx[0].vin[0].scriptSig);

        // Prebuild hash buffers
        char pdata[128];
        FormatHashBuffers(pblock, pdata);

        uint256 hashTarget = CBigNum().SetCompact(pblock->nBits).getuint256();

        CTransaction coinbaseTx = pblock->vtx[0];
        std::vector<uint256> merkle = pblock->GetMerkleBranch(0);

        Object result;
        result.push_back(Pair("data",     HexStr(BEGIN(pdata), END(pdata))));
        result.push_back(Pair("target",   HexStr(BEGIN(hashTarget), END(hashTarget))));

        CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
        ssTx << coinbaseTx;
        result.push_back(Pair("coinbase", HexStr(ssTx.begin(), ssTx.end())));

        Array merkle_arr;

        BOOST_FOREACH (uint256 merkleh, merkle) {
            printf("%s\n", merkleh.ToString().c_str());
            merkle_arr.push_back(HexStr(BEGIN(merkleh), END(merkleh)));
        }

        result.push_back(Pair("merkle", merkle_arr));

        return result;
    }
    else
    {
        // Parse parameters
        vector<unsigned char> vchData = ParseHex(params[0].get_str());
        vector<unsigned char> coinbase;

        if(params.size() == 2)
            coinbase = ParseHex(params[1].get_str());

        if (vchData.size() != 128)
            throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid parameter");

        CBlock* pdata = (CBlock*)&vchData[0];

        // Byte reverse
        for (int i = 0; i < 128/4; i++)
            ((unsigned int*)pdata)[i] = ByteReverse(((unsigned int*)pdata)[i]);

        // Get saved block
        if (!mapNewBlock.count(pdata->hashMerkleRoot))
            return false;
        CBlock* pblock = mapNewBlock[pdata->hashMerkleRoot].first;

        pblock->nTime = pdata->nTime;
        pblock->nNonce = pdata->nNonce;
        pblock->nPoWHeight = pdata->nPoWHeight;
        pblock->nOption = pdata->nOption;
        pblock->nPoSHeight = pdata->nPoSHeight;
        pblock->nPoSPot = pdata->nPoSPot;

        if(coinbase.size() == 0)
            pblock->vtx[0].vin[0].scriptSig = mapNewBlock[pdata->hashMerkleRoot].second;
        else
            CDataStream(coinbase, SER_NETWORK, PROTOCOL_VERSION) >> pblock->vtx[0];

        pblock->hashMerkleRoot = pblock->BuildMerkleTree();

        return CheckWork(pblock, *pwalletMain, reservekey);
    }
}

//
// TODO : need to return the last PoW infomation
//        miner will keep mining the previous(lastest) hash
//

Value getwork(const Array& params, bool fHelp)
{
    if (fHelp || params.size() > 1)
        throw runtime_error(
            "getwork [data]\n"
            "If [data] is not specified, returns formatted hash data to work on:\n"
            "  \"midstate\" : precomputed hash state after hashing the first half of the data (DEPRECATED)\n" // deprecated
            "  \"data\" : block data\n"
            "  \"hash1\" : formatted hash buffer for second hash (DEPRECATED)\n" // deprecated
            "  \"target\" : little endian hash target\n"
            "If [data] is specified, tries to solve the block and returns true if it was successful.");

    if (vNodes.empty())
        throw JSONRPCError(RPC_CLIENT_NOT_CONNECTED, "JackpotCoin is not connected!");

    if (IsInitialBlockDownload())
        throw JSONRPCError(RPC_CLIENT_IN_INITIAL_DOWNLOAD, "JackpotCoin is downloading blocks...");

    typedef map<uint256, pair<CBlock*, CScript> > mapNewBlock_t;
    static mapNewBlock_t mapNewBlock;
    static vector<CBlock*> vBlock;

    if (params.size() == 0)
    {
        // Update block
        static unsigned int nTransactionsUpdatedLast;
        static CBlockIndex* pindexPrev;
        static int64 nStart;
        static CBlock* pblock;
        if (pindexPrev != pindexBest ||
            (nTransactionsUpdated != nTransactionsUpdatedLast && GetTime() - nStart > 60))
        {
            if (pindexPrev != pindexBest)
            {
                // Deallocate old blocks since they're obsolete now
                mapNewBlock.clear();
                BOOST_FOREACH (CBlock* pblock, vBlock)
                    delete pblock;
                vBlock.clear();
            }

            // Clear pindexPrev so future getworks make a new block, despite any failures from here on
            pindexPrev = NULL;

            // Store the pindexBest used before CreateNewBlock, to avoid races
            nTransactionsUpdatedLast = nTransactionsUpdated;
            CBlockIndex* pindexPrevNew = pindexBest;
            nStart = GetTime();

            // Create new block
            CBlock *pblock = new CBlock();
            if (!pblock->CreateWithKey(pwalletMain, *pMiningKey, false))
                throw JSONRPCError(RPC_MISC_ERROR, "Cannot create a block");
            vBlock.push_back(pblock);

            // Need to update only after we know CreateNewBlock succeeded
            pindexPrev = pindexPrevNew;
        }

        // Update nTime
        pblock->UpdateTime(pindexPrev);
        pblock->nNonce = 0;

        // Update nExtraNonce
        static unsigned int nExtraNonce = 0;
        IncrementExtraNonce(pblock, pindexPrev, nExtraNonce);

        // Save
        mapNewBlock[pblock->hashMerkleRoot] = make_pair(pblock, pblock->vtx[0].vin[0].scriptSig);

        // Pre-build hash buffers
        char pdata[128];
        FormatHashBuffers(pblock, pdata);

        uint256 hashTarget = CBigNum().SetCompact(pblock->nBits).getuint256();

        Object result;
        result.push_back(Pair("data",     HexStr(BEGIN(pdata), END(pdata))));
        result.push_back(Pair("target",   HexStr(BEGIN(hashTarget), END(hashTarget))));
        return result;
    }
    else
    {
        // Parse parameters
        vector<unsigned char> vchData = ParseHex(params[0].get_str());
        if (vchData.size() != 128)
            throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid parameter");
        CBlock* pdata = (CBlock*)&vchData[0];

        // Byte reverse
        for (int i = 0; i < 128/4; i++)
            ((unsigned int*)pdata)[i] = ByteReverse(((unsigned int*)pdata)[i]);

        // Get saved block
        if (!mapNewBlock.count(pdata->hashMerkleRoot))
            return false;
        CBlock* pblock = mapNewBlock[pdata->hashMerkleRoot].first;

        pblock->nTime = pdata->nTime;
        pblock->nNonce = pdata->nNonce;
        pblock->nPoWHeight = pdata->nPoWHeight;
        pblock->nOption = pdata->nOption;
        pblock->nPoSHeight = pdata->nPoSHeight;
        pblock->nPoSPot = pdata->nPoSPot;
        pblock->vtx[0].vin[0].scriptSig = mapNewBlock[pdata->hashMerkleRoot].second;
        pblock->hashMerkleRoot = pblock->BuildMerkleTree();
 
        assert(pwalletMain != NULL);
        return CheckWork(pblock, *pwalletMain, *pMiningKey);
    }
}


Value getblocktemplate(const Array& params, bool fHelp)
{
    if (fHelp || params.size() > 1)
        throw runtime_error(
            "getblocktemplate [params]\n"
            "Returns data needed to construct a block to work on:\n"
            "  \"version\" : block version\n"
            "  \"previousblockhash\" : hash of current highest block\n"
            "  \"transactions\" : contents of non-coinbase transactions that should be included in the next block\n"
            "  \"coinbaseaux\" : data that should be included in coinbase\n"
            "  \"coinbasevalue\" : maximum allowable input to coinbase transaction, including the generation award and transaction fees\n"
            "  \"superblock\" : last super block index\n"
            "  \"roundmask\" : option of block [luckynumber][bet][option]\n"
            "  \"possuperblock\" : last super block index for PoS\n"
            "  \"pospot\" : amount of pot for PoS\n"
            "  \"target\" : hash target\n"
            "  \"mintime\" : minimum timestamp appropriate for next block\n"
            "  \"curtime\" : current timestamp\n"
            "  \"mutable\" : list of ways the block template may be changed\n"
            "  \"noncerange\" : range of valid nonces\n"
            "  \"sigoplimit\" : limit of sigops in blocks\n"
            "  \"sizelimit\" : limit of block size\n"
            "  \"bits\" : compressed target of next block\n"
            "  \"height\" : height of the next block\n"
            "See https://en.bitcoin.it/wiki/BIP_0022 for full specification.");

    std::string strMode = "template";
    if (params.size() > 0) 
    {
        const Object& oparam = params[0].get_obj();
        const Value& modeval = find_value(oparam, "mode");
        if (modeval.type() == str_type)
            strMode = modeval.get_str();
        else if (modeval.type() == null_type) 
        {
            /* Do nothing */
        }
        else 
            throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid mode");
    }

    if (strMode != "template")
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid mode");

    if (vNodes.empty())
        throw JSONRPCError(RPC_CLIENT_NOT_CONNECTED, "JackpotCoin is not connected!");

    if (IsInitialBlockDownload())
        throw JSONRPCError(RPC_CLIENT_IN_INITIAL_DOWNLOAD, "JackpotCoin is downloading blocks...");

    // Update block
    static unsigned int nTransactionsUpdatedLast;
    static CBlockIndex* pindexPrev;
    static int64 nStart;
    static CBlock* pblock;
    if (pindexPrev != pindexBest ||
        (nTransactionsUpdated != nTransactionsUpdatedLast && GetTime() - nStart > 5))
    {
        // Clear pindexPrev so future calls make a new block, despite any failures from here on
        pindexPrev = NULL;

        // Store the pindexBest used before CreateNewBlock, to avoid races
        nTransactionsUpdatedLast = nTransactionsUpdated;
        CBlockIndex* pindexPrevNew = pindexBest;
        nStart = GetTime();

        // Create new block
        if (pblock)
        {
            delete pblock;
            pblock = NULL;
        }

        // Create new block
        CBlock *pblock = new CBlock();
        CScript scriptDummy = CScript() << OP_TRUE;
        if (!pblock->Create(pwalletMain,scriptDummy, false))
            throw JSONRPCError(RPC_MISC_ERROR, "Cannot create a block");

        // Need to update only after we know CreateNewBlock succeeded
        pindexPrev = pindexPrevNew;
    }

    // Update nTime
    pblock->UpdateTime(pindexPrev);
    pblock->nNonce = 0;

    Array transactions;
    map<uint256, int64_t> setTxIndex;
    int i = 0;
    BOOST_FOREACH (CTransaction& tx, pblock->vtx)
    {
        uint256 txHash = tx.GetHash();
        setTxIndex[txHash] = i++;

        if (tx.IsCoinBaseOrStake())
            continue;

        Object entry;

        CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
        ssTx << tx;
        entry.push_back(Pair("data", HexStr(ssTx.begin(), ssTx.end())));
        entry.push_back(Pair("hash", txHash.GetHex()));

        Array deps;
        BOOST_FOREACH (const CTxIn &in, tx.vin)
        {
            if (setTxIndex.count(in.prevout.hash))
                deps.push_back(setTxIndex[in.prevout.hash]);
        }
        entry.push_back(Pair("depends", deps));

        transactions.push_back(entry);
    }

    Object aux;
    aux.push_back(Pair("flags", HexStr(COINBASE_FLAGS.begin(), COINBASE_FLAGS.end())));

    uint256 hashTarget = CBigNum().SetCompact(pblock->nBits).getuint256();

    static Array aMutable;
    if (aMutable.empty())
    {
        aMutable.push_back("time");
        aMutable.push_back("transactions");
        aMutable.push_back("prevblock");
    }

    Object result;
    result.push_back(Pair("version", pblock->nVersion));
    result.push_back(Pair("previousblockhash", pblock->hashPrevBlock.GetHex()));
    result.push_back(Pair("transactions", transactions));
    result.push_back(Pair("coinbaseaux", aux));
    result.push_back(Pair("coinbasevalue", (int64)pblock->vtx[0].vout[0].nValue));
    result.push_back(Pair("superblock", (int64)(pblock->nPoWHeight)));
    result.push_back(Pair("roundmask", (int64)(pblock->nOption)));
    result.push_back(Pair("possuperblock", (int64)(pblock->nPoSHeight)));
    result.push_back(Pair("pospot", (int64)(pblock->nPoSPot)));
    result.push_back(Pair("target", hashTarget.GetHex()));
    result.push_back(Pair("mintime", (int64)pindexPrev->GetMedianTimePast() + 1));
    result.push_back(Pair("mutable", aMutable));
    result.push_back(Pair("noncerange", "00000000ffffffff"));
    result.push_back(Pair("sigoplimit", (int64)MAX_BLOCK_SIGOPS));
    result.push_back(Pair("sizelimit", (int64)MAX_BLOCK_SIZE));
    result.push_back(Pair("curtime", (int64)pblock->nTime));
    result.push_back(Pair("bits", HexBits(pblock->nBits)));
    result.push_back(Pair("height", (int64)(pindexPrev->nHeight + 1)));

    return result;
}

Value submitblock(const Array& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 2)
        throw runtime_error(
            "submitblock <hex data> [optional-params-obj]\n"
            "[optional-params-obj] parameter is currently ignored.\n"
            "Attempts to submit new block to network.\n"
            "See https://en.bitcoin.it/wiki/BIP_0022 for full specification.");

    vector<unsigned char> blockData(ParseHex(params[0].get_str()));
    CDataStream ssBlock(blockData, SER_NETWORK, PROTOCOL_VERSION);
    CBlock pblock;
    try 
    {
        ssBlock >> pblock;
    }
    catch (std::exception &e) {
        throw JSONRPCError(RPC_DESERIALIZATION_ERROR, "Block decode failed");
    }
 
    CValidationState state;
    bool fAccepted = ProcessBlock(state, NULL, &pblock);
    if (!fAccepted)
        return "rejected"; // TODO: report validation state

    return Value::null;
}
