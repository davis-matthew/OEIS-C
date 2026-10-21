/*
 * A283556  Digital root of the sum of the first n primes.
 * https://oeis.org/A283556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((sum of first n primes - 1) mod 9) + 1, with a(0)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* Cache of the first primes and the running sum modulo 9. */
    static uint32_t *primes = NULL;
    static uint8_t *prefix_mod9 = NULL;
    static size_t cached_n = 0;          /* number of primes cached */

    /* Grow the cache if a larger n is requested. */
    if ((size_t)n > cached_n) {
        uint32_t *new_primes = NULL;
        size_t new_cnt;

        /* Generate the first n primes. */
        sieve_first_n_primes(n, &new_primes, &new_cnt);

        /* Build a new prefix array of sums modulo 9. */
        uint8_t *new_prefix = (uint8_t *)malloc((n + 1) * sizeof(uint8_t));
        if (!new_prefix) {
            /* Allocation failed – give up. */
            free(new_primes);
            return (seq_t)0;
        }
        new_prefix[0] = 0;
        for (size_t i = 1; i <= n; ++i) {
            uint32_t p = new_primes[i - 1];
            new_prefix[i] = (new_prefix[i - 1] + (p % 9)) % 9;
        }

        /* Replace the old cache. */
        free(primes);
        free(prefix_mod9);
        primes = new_primes;
        prefix_mod9 = new_prefix;
        cached_n = n;
    }

    /* a(0) is defined to be 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Digit root of the sum: 9 if the sum is a non‑zero multiple of 9,
       otherwise the remainder itself. */
    uint8_t rem = prefix_mod9[(size_t)n];
    return (rem == 0) ? (seq_t)9 : (seq_t)rem;
}
