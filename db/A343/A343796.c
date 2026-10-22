/*
 * A343796  a(n) is the number of distinct primes of the form A007504(n) mod p
 *          for the first n primes p.
 * https://oeis.org/A343796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 333, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of distinct primes r such that r ≡ S_n (mod p) for
   some p among the first n primes, where S_n = sum of the first n primes. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    /* Retrieve the first n primes */
    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);   /* primes[0..n-1] */

    /* Compute the sum S_n of the first n primes */
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i)
        sum += (seq_t)primes[i];

    /* Allocate a bit array to mark which residues (primes) have appeared */
    uint32_t max_prime = primes[n - 1];
    size_t seen_size = (size_t)max_prime + 1;
    uint8_t *seen = (uint8_t *)calloc(seen_size, sizeof(uint8_t));
    seq_t distinct = 0;

    /* For each prime p, consider the residue S_n mod p */
    for (idx_t i = 0; i < n; ++i) {
        uint32_t p = primes[i];
        seq_t r = sum % (seq_t)p;

        if (r > 1) {                                  /* ignore 0 and 1 */
            uint32_t rv = (uint32_t)r;                 /* r < p <= max_prime < 2^32 */
            if (is_prime_small(rv)) {
                if (!seen[rv]) {
                    seen[rv] = 1;
                    ++distinct;
                }
            }
        }
    }

    free(seen);
    free(primes);
    return distinct;
}
