/*
 * A280872  Primes that have exactly 7 zeros and 7 ones in their binary
 *          expansion.
 * https://oeis.org/A280872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th prime whose binary representation has exactly 7
 *            zeros and 7 ones (i.e. 14 bits total)
 * Range:     u8 none, u16 n <= 189, u32 n <= 189, u64 n <= 189, u128 n <= 189,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;      /* array of found primes */
    static size_t   primes_cnt = 0;   /* number of primes stored   */
    static bool     initialized = false;

    if (!initialized) {
        const uint64_t low  = (1ULL << 13);          /* 8192 */
        const uint64_t high = (1ULL << 14) - 1ULL;   /* 16383 */

        /* The maximum number of 14-bit numbers with 7 ones is C(13,6)=1716. */
        const size_t max_candidates = 1716;
        primes = (seq_t *)malloc(max_candidates * sizeof(seq_t));
        if (!primes) {            /* allocation failed – give up */
            initialized = true;
            return (seq_t)0;
        }

        primes_cnt = 0;

        for (uint64_t v = low; v <= high; ++v) {
            /* Count the number of 1‑bits. */
            uint64_t t = v;
            int pc = 0;
            while (t) {
                t &= t - 1;
                ++pc;
            }
            if (pc != 7) continue;         /* not 7 ones */

            /* For 14‑bit values a small primality routine is sufficient. */
            if (is_prime_small((uint64_t)v)) {
                primes[primes_cnt++] = (seq_t)v;
            }
        }

        /* Shrink to the exact number of primes found. */
        if (primes_cnt < max_candidates) {
            seq_t *tmp = (seq_t *)realloc(primes, primes_cnt * sizeof(seq_t));
            if (tmp) primes = tmp;
        }

        initialized = true;
    }

    if (n == 0) return (seq_t)0;
    if ((idx_t)(n - 1) < primes_cnt) return primes[n-1];
    return (seq_t)0;
}
