import { ethers, solidityPackedKeccak256 } from "ethers";

// Generate a random private key and create a wallet from it
const pkey = ethers.Wallet.createRandom().privateKey;
const wallet = new ethers.Wallet(pkey);

// Generate a random salt
const salt = ethers.randomBytes(32); // 32 bytes of random data
const tokenId = '5';
const address = wallet.address;
const amount = '0';

(async () => {
    // Create a salted token ID hash
    const saltedTokenId = solidityPackedKeccak256(['bytes32', 'uint'], [salt, tokenId]);

    // Create a hash of the message to be signed
    const hash = solidityPackedKeccak256(['address', 'bytes32', 'uint'], [address, saltedTokenId, amount]);

    // Sign the message
    const signature = await wallet.signMessage(ethers.toBeArray(hash));
    
    // Log the results
    console.log("Salt:", ethers.hexlify(salt)); // Display the random salt
    console.log("Salted Token ID:", ethers.hexlify(saltedTokenId)); // Display the salted token ID
    console.log("Signature:", signature);
})();
