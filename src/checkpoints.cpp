// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

 

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (     0, uint256("0x8b6d9062479865e08c9d1218a3b9dfbd2ecf42e9b5d710e909b6ecb7e3ac1175"))
        (  1001, uint256("0x0000000000d32caf989751749fbdbf5acd6db8eeb50b30c7f9ebc712a467a688"))
        ( 10011, uint256("0xfe03bc65a1cb456f2160794d03857683027129556640fe97558b5cf88d9d8490"))
        (100001, uint256("0x617ca93a457f15e9c76a669e864f5eaa314c89cad8a4cc3b92e133b6b4c0b745"))
        (150001, uint256("0x936a6c5ffe987dfb3d6ada997063e799f6aad0c52a0ed01d2907a40644347d63"))
        (184001, uint256("0xe8738405a01d2924a0f70b249ecdceaec22d8c12416084d3f5c0a57f8358ed8b"))
        (250001, uint256("0xefb5e113931d6ce197fe05af3b552983c349af562c0c5cb18106b2c0cf885119"))
        (370001, uint256("0x9e68c44b4d3f99b4b5a7d8367ae9fd2500bc79a7814970f5b16c6def99ce2ab1"))
        (448650, uint256("0x000000000005edf95d598f925586364013fc143ee31044fc7cc598b321b499e5"))
        (500001, uint256("0x49b3e63d4f092c6bab8dc938fe3ba6b0316d866dc3e6f3331c0ff29640f8ed54"))
        (550001, uint256("0x65cba891ff6ffd4b6ac3955b1ee722f321dc77567fdd190f6a415998e7f2cf04"))
        (600001, uint256("0xacd60c624540af2e1b03c98b021cbd7ec1802b3fffe25a349130a7b95fb09ed6"))
        (700001, uint256("0xe21e3baf02626294097ed870d41d060ad5b9a286e5418365ff0f879f8440f769"))
        (800001, uint256("0x0000000000770eeaa628e9fac8acc4f62c7697daa46ba959345e157cb6cbc88a"))
        (850001, uint256("0x60b09745de56e8ced1e9aa858e39ab435e848fd30d29252072904b1394ee0286"))
        ;

    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        (     0, uint256("0x8b6d9062479865e08c9d1218a3b9dfbd2ecf42e9b5d710e909b6ecb7e3ac1175"))
        ;


    bool CheckBlock(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

   // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {

        if (pindex == NULL)
            return 0.0;

        double nHeightBefore =  pindex->nHeight;
        double nHeightAfter  = (time(NULL) - pindex->nTime) / 17;
        return (nHeightBefore / (nHeightBefore + nHeightAfter));
    }


    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH (const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
