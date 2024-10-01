#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>

// Function to generate a random value between min and max (inclusive)
void generate_random_in_range(mpz_t result, mpz_t min, mpz_t max) {
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, rand());

    // Generate a random number in the range [min, max]
    mpz_sub(result, max, min);       // result = max - min
    mpz_urandomm(result, state, result); // result = random number in [0, max - min)
    mpz_add(result, result, min);    // result = result + min to shift to [min, max]

    gmp_randclear(state);
}

// Function to simulate minting a star to an address
void spawn_star(int star_id) {
    printf("Spawning star with ID %d\n", star_id);
}

// Function to generate a commitment to a secret value
void generate_commitment(mpz_t commitment, mpz_t secret, mpz_t generator, mpz_t modulus) {
    mpz_powm(commitment, generator, secret, modulus);
}

// Function to prove that a committed value is in the range [min, max]
int prove_range_with_zkp(mpz_t secret, mpz_t min, mpz_t max, mpz_t modulus, mpz_t generator) {
    mpz_t commitment, min_commitment, max_commitment, proof;

    mpz_inits(commitment, min_commitment, max_commitment, proof, NULL);

    // Generate commitment to the secret value
    generate_commitment(commitment, secret, generator, modulus);
    // gmp_printf("Commitment: %Zd\n", commitment);

    // Commit to the minimum and maximum values
    generate_commitment(min_commitment, min, generator, modulus);
    generate_commitment(max_commitment, max, generator, modulus);

    // Proof would be a comparison between the commitment and min/max commitments.
    // In a full implementation, you would calculate and return the proof to be verified.
    // For simplicity, we just compare the secret directly here.
    int is_valid = (mpz_cmp(secret, min) >= 0) && (mpz_cmp(secret, max) <= 0);

    // Clean up
    mpz_clears(commitment, min_commitment, max_commitment, proof, NULL);
    return is_valid;
}

// Main function to demonstrate spawning stars based on ZKP range proof
int main() {
    // Initialize GMP integers for range and generator
    mpz_t min, max, secret, generator, modulus, selected_star;
    mpz_inits(min, max, secret, generator, modulus, selected_star, NULL);

    // Set the range for star IDs (for example, 1 to 100)
    mpz_set_ui(min, 1);
    mpz_set_ui(max, 100);

    // Generate a secret value (this should be securely generated in practice)
    mpz_set_ui(secret, 50); // Example secret value within the range

    // Set the generator and modulus (these would be based on your cryptographic scheme)
    mpz_set_ui(generator, 2);
    mpz_set_str(modulus, "101", 10);  // A small prime modulus for demonstration

    // Prove that the secret value is within the specified range
    if (prove_range_with_zkp(secret, min, max, modulus, generator)) {
        printf("Secret value is within the range [%Zd, %Zd].\n", min, max);

        // Generate a random star ID within the specified range
        generate_random_in_range(selected_star, min, max);
        gmp_printf("Selected star ID: %Zd\n", selected_star);

        // Spawn the star based on the random selection
        spawn_star(mpz_get_ui(selected_star)); // Convert selected_star to unsigned int for spawning
    } else {
        printf("Secret value is outside the range [%Zd, %Zd].\n", min, max);
    }

    gmp_printf("Retrying with bad range\n");

    // Set the range for star IDs (for example, 1 to 100)
    mpz_set_ui(min, 1);
    mpz_set_ui(max, 100);

    // Generate a secret value (this should be securely generated in practice)
    mpz_set_ui(secret, 501); // Example secret value within the range

    // Set the generator and modulus (these would be based on your cryptographic scheme)
    mpz_set_ui(generator, 2);
    mpz_set_str(modulus, "101", 10);
    // Prove that the secret value is within the specified range
    if (prove_range_with_zkp(secret, min, max, modulus, generator)) {
        printf("Secret value is within the range [%Zd, %Zd].\n", min, max);

        // Generate a random star ID within the specified range
        generate_random_in_range(selected_star, min, max);
        gmp_printf("Selected star ID: %Zd\n", selected_star);

        // Spawn the star based on the random selection
        spawn_star(mpz_get_ui(selected_star)); // Convert selected_star to unsigned int for spawning
    } else {
        printf("Secret value is outside the range [%Zd, %Zd].\n", min, max);
    }

    // Clear memory
    mpz_clears(min, max, secret, generator, modulus, selected_star, NULL);
    return 0;
}
