JackpotCoin (JACKPOT) 

- PoW/PoS independent
- 3 transaction confirmations - average less than 1 min confirmation
- 70 minted block confirmations
- Total coins will be approximately 30 billions.

PoW:
- Jackpot Hash Algorithm (JHA - a variation of SHA-3 algorithm)
- Minable by both CPU and GPU, both CPU miner and GPU miner are provided
- 120 sec block target
- diff retarget each block
- Initial payout will be 100000 coins per block
- True random superblock 
- Superblock payout (aka the jackpot) will starts at 10X normal block payout, and grows parabolically when the distance with last superblock increases. The average interval between superblocks will be 3.5 days. The payout starts at 10X normal payout, grows to 100X at 3.5 days, and then grows to 1000X at 7 days (if the superblock not triggered). After 7 days, the payout will stay at 1000X, but at the end of the 8th day if still no superblock is triggered by the random number, the system will gradually increase the chance for a superblock until a 1000x superblock is triggered.
- block payout will be reduced 10% each week (will be halved at approximately each 7 weeks).
- minimum payout for PoW will be 1 coin/block (will be reached after 2+ years)

PoS:
- 20 sec block target
- diff retarget each block
- PoS daily interest payout initially at 0.1% per day (equivalent to 44% annual interest rate), gradually dropping down to 0.01% per day (equivalent to 3.7% annual rate), the decrease will be over a period of 5 years (rate adjusted each 3 months). Then it will stay at this rate.
- minimum holding time before the pos will be generated: 1 day.
- max accumulated coin-day is 100 days (after 100 days, only the 100-day coin-day will be counted).

1% premine to cover development costs (website, server, development), code maintenance/enhancements, bounties, shops/games, faucets/giveaways etc.


Ports:
connection:	15371
RPC:		15372
