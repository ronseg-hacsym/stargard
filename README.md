# stargard
privacy provisioning for star funding the urbit ecosystem

1) prove a galaxy point as a zk proof in c `$ ./galaxy/ownership`
2) spawn stars off-chain in c by proving with zkp a range of values and if valid select with a random number (where a random number oracle can be replaced) `$ ./star/range_proof`
3) generate a proof for the spawning of that star to redeem for erc20 tokens onchain `$ ts-node ./optionality/evm/ERC20/ts/generate_signature.ts` -> `./optionality/evm/ERC20/sol/ERC20.sol`
4) redeem star by off-chain writing to chain in step 2) from central funder, then on-chain proving `?` -> `./ERC721/sol/MockAzimuthWrapper.sol`