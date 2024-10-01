#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Function to generate a random value between 0 and limit - 1
void generate_random(mpz_t result, mpz_t limit) {
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, rand());
    mpz_urandomm(result, state, limit);
    gmp_randclear(state);
}

// Function to simulate minting a star to an address
void mint_star(const char *address, int star_id) {
    printf("Minting star ID %d to address: %s\n", star_id, address);
}

// Function to perform a single zk-proof for minting a star based on galaxy ownership token ID
int prove_star_ownership(int galaxy_token_id, mpz_t modulus, mpz_t generator, const char *address) {
    mpz_t x, y, r, t, c, z, left, right, temp, p_minus_1, combined_value;
    mpz_inits(x, y, r, t, c, z, left, right, temp, p_minus_1, combined_value, NULL);

    // Compute (modulus - 1)
    mpz_sub_ui(p_minus_1, modulus, 1);

    // Secret x for the galaxy owner (prover's secret value); this should be known to the owner
    // For simplicity, we use a fixed value, but this should be dynamically generated
    mpz_set_str(x, "20", 10); // Prover's secret (this would be different for different galaxies)

    // Combine the galaxy token ID with the secret x to form a unique value
    mpz_mul_ui(combined_value, x, galaxy_token_id);  // combined_value = x * galaxy_token_id

    // Prover computes y = g^combined_value mod modulus
    mpz_powm(y, generator, combined_value, modulus);

    // Prover chooses a random r
    generate_random(r, p_minus_1);

    // Compute t = g^r mod modulus (commitment)
    mpz_powm(t, generator, r, modulus);
    gmp_printf("Prover sends commitment t = %Zd\n", t);

    // Verifier sends a random challenge c
    generate_random(c, modulus);
    gmp_printf("Verifier sends challenge c = %Zd\n", c);

    // Prover computes z = (r + c * combined_value) mod (modulus - 1)
    mpz_mul(temp, c, combined_value);  // temp = c * combined_value
    mpz_add(z, r, temp);               // z = r + c * combined_value
    mpz_mod(z, z, p_minus_1);          // z mod (modulus - 1)
    gmp_printf("Prover sends response z = %Zd\n", z);

    // Verifier checks if g^z ≡ t * y^c mod modulus
    mpz_powm(left, generator, z, modulus);    // left = g^z mod modulus
    mpz_powm(right, y, c, modulus);           // right = y^c mod modulus
    mpz_mul(right, right, t);                 // right = t * y^c
    mpz_mod(right, right, modulus);           // right mod modulus

    gmp_printf("Verifier computes left = %Zd, right = %Zd\n", left, right);

    // Check if the proof is valid
    if (mpz_cmp(left, right) == 0) {
        printf("Verification successful for galaxy token ID %d! Prover knows the secret x.\n", galaxy_token_id);
        mint_star(address, galaxy_token_id); // Mint the star to the address
        mpz_clears(x, y, r, t, c, z, left, right, temp, p_minus_1, combined_value, NULL);
        return 1;
    } else {
        printf("Verification failed for galaxy token ID %d! Prover does not know the secret x.\n", galaxy_token_id);
        mpz_clears(x, y, r, t, c, z, left, right, temp, p_minus_1, combined_value, NULL);
        return 0;
    }
}

// Main function to prove ownership for a single galaxy ownership token ID
int main() {
    // Initialize random seed
    srand(time(NULL));

    // Initialize GMP integers for modulus and generator
    mpz_t modulus, generator;
    mpz_inits(modulus, generator, NULL);

    // Set prime modulus and generator (use reasonably large values for real applications)
    mpz_set_str(modulus, "101", 10);  // Example prime modulus
    mpz_set_str(generator, "2", 10);   // Example generator

    // Specify the address to mint stars to
    const char *address = "0x123456789ABCDEF";

    // Galaxy ownership token ID (use a specific ID instead of multiple stars)
    int galaxy_token_id = 1; // Example galaxy token ID

    // Perform zk-proof for minting the star based on galaxy ownership
    printf("\n--- Proving ownership and minting for galaxy token ID %d ---\n", galaxy_token_id);
    if (!prove_star_ownership(galaxy_token_id, modulus, generator, address)) {
        printf("Failed to mint star ID %d due to failed proof.\n", galaxy_token_id);
    }

    // Clear memory
    mpz_clears(modulus, generator, NULL);

    return 0;
}
