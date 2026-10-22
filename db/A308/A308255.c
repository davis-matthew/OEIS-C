/*
 * A308255  Sum of the smallest side lengths of all integer-sided triangles
 *          with perimeter n and nonprime sides.
 * https://oeis.org/A308255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of the smallest side lengths of all integer‑sided triangles
   with perimeter n and nonprime sides.
   For each k ≤ i ≤ j with k + i + j = n, if k, i, j are not prime
   and i + k > j, add k to the sum. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle small n explicitly */
    if (n < 3) return 0;

    /* Ensure we have an up‑to‑date array of prime flags up to n. */
    static idx_t prime_limit = 0;
    static bool *is_prime = NULL;      /* true if prime, false otherwise */

    if (n + 1 > prime_limit) {
        idx_t new_limit = n + 1;
        bool *prime = (bool*)malloc(new_limit * sizeof(bool));
        if (!prime) return 0;               /* allocation failure */

        /* Initially assume all numbers ≥2 are prime */
        for (idx_t i = 0; i < new_limit; ++i) prime[i] = true;
        if (new_limit > 0) prime[0] = false;
        if (new_limit > 1) prime[1] = false;

        /* Sieve of Eratosthenes */
        for (idx_t p = 2; (idx_t)p * p < new_limit; ++p) {
            if (prime[p]) {
                for (idx_t mult = p * p; mult < new_limit; mult += p)
                    prime[mult] = false;
            }
        }

        /* Replace the old array */
        free(is_prime);
        is_prime = prime;
        prime_limit = new_limit;
    }

    seq_t sum = 0;
    idx_t max_k = n / 3;                    /* k ranges 1 .. floor(n/3) */

    for (idx_t k = 1; k <= max_k; ++k) {
        if (is_prime[k]) continue;           /* side k must be nonprime */

        idx_t max_i = (n - k) / 2;           /* i ranges k .. floor((n-k)/2) */
        for (idx_t i = k; i <= max_i; ++i) {
            if (is_prime[i]) continue;       /* side i must be nonprime */

            idx_t j = n - i - k;             /* j is the remaining side */

            if (j < i) continue;             /* ensure ordering k <= i <= j */
            if (is_prime[j]) continue;       /* side j must be nonprime */

            /* Triangle inequality: i + k > j */
            if (i + k <= j) continue;

            sum += (seq_t)k;                 /* add the smallest side */
        }
    }
    return sum;
}
