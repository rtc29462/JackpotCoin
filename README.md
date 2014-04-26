JackpotCoin (JACKPOT) 

JackpotCoin is a PoW/PoS coin that is designed to be the unit of cybercoin. It has a total of 50 billion unit to be released, through a fair p2p mining process. JackpotCoin is from the family of PPCoin/Novacoin with latest available algorithms that fixed various problems in the PoW/PoS coins. As compared to the PoW only coins, the proof of stake coins are more resilient to 51% attack. Also it is a long term energy-efficient crypto-currency since not all coins are obtained by mining.

Each grain block provides random 1024 - 2048 grains initially. It also provides rare superblocks. The coin uses hash-based algorithm to determine superblocks based on the number of 9s in the previous block hash. It works as follows:
- the hash contains six 9s or less: regular block
- the hash contains seven or eight 9s: 8 times the regular block (about 5% chances)
- the hash contains nine or more 9s: 64 times the regular block (about 1% chances)

JackpotCoin also provides 5% annual interest on the grained held. The interest will be paid about every 2 weeks.

Specifications:
	- 30 seconds block target
	- 1024-2048 grains per block for normal blocks
	- 8 times or 64 times the regular payout per block for super blocks depends on the number of 9s in the previous hash
	- Difficulty retargets every block 
	- mining payout will be halved every 90 days (259200 blocks)
	- minimum payout per block 1 coin
	- Total grains will be 50 billions
	- 6 confirmations for transaction
	- 50 confirmations for minted blocks

	- Ports: 11054 (connection) and 11055 (RPC)


Reward Sytem 

   [TBD]
   [REASONABLE NUMBER OF COINS; NOT TOO MUCH, NOT TOO SMALL, BUT BIG REWARD]
   

PoW/PoS Hybrid
   
   [TBD]



Hybrid Dual Block Chain 

   PoW and PoS block chains are oganized within a block chain but managed independantly. 
   It makes more stable operation of hybid blocks without some Diff control or 
   Unreasonable block time. 
   
   Two semi-independant block chains are support two major advantages. 
   
   Stable large coin distribution with PoW Chain
   Stable Fast coin transaction with PoS Chain

   Most of TxFee and Interest will be paid to PoS miner, it is a fair way for the network
   maintainers.
   


AdvSHA3

   We are using the customized and optimized very secure hashing method for this coin, 
   It is base on SHA3 and Its candidates (BLAKE, JH, GROESTL, SKEIN). 

   1 round SHA3 (Keccak) input hashing with 
   9 rounds random using 4  candidates hashing methods are applied,
   
   It is the most secure hashing method, using ALU and Internal Memory of GPU to reduce
   the power consumption. and CPU frendly method. 
  
   for The ASIC, the cost of ASIC is ~10x more than the general scrypt ASIC due to 
   the complicity of code and RAM/CACHE requirments, it's hard to apply on ASIC quickly.
   We are estimating it will takes ~5 years to appy on ASIC with the reasonable cost.
   It means, this method has ~5 years ASIC resist, 
   
   It is the fair way to support CPU/GPU miner at beginning, and 
   the fair way to support ASIC at the network maintenance stage.
   

   
Super Block Reward with Anti-Cheating

   To avoid the cheating miner, true random reward system has applied. 
   In this system, the super block rewards are distributed to the super block hash generator
   instead of finder. so, cheating miner can not archive the selective mining 
   to hurt the super block only.
   
   The rewards are depend on the period from the previous super block, 
   If no one find the super block over long time, the rewards will be increasing,
   
   (1) constant reward over time
   (2) various reward depend on the luck
   
   Note : the status of superblock reward (such as possible rewards) will be couting on 
          website in realtime,
          
          

Fair Pre-mined and Open Public [3.0%]

   The usage of pre-mined coin will as below 
   
   Anti-dump fund   2.0%   (Pre Sales with BTC in Public)
   
   1st Exchanges    0.1%
   2nd Exchanges    0.1%
   
   1st Pool         0.1%
   2nd Pool         0.1%
   
   Others           0.6%
   
