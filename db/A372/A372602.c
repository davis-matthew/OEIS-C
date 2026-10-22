/*
 * A372602  The maximal exponent in the prime factorization of the largest
 *          square dividing n.
 * https://oeis.org/A372602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS Formula: a(n) = maximal exponent in the prime factorization
   of the largest square dividing n.
   Algorithm: factor n, for each prime exponent e compute 2*floor(e/2)
   and keep the maximum.  Returns 0 if no prime has exponent ≥2.
   Complexity: O(√n) per call, adequate for n up to ~10⁶ and comfortably
   below the 2‑second limit for n ≈ 10 000.
*/

#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto        */
#include "../utils/floor_sqrt.h"          /* floor_sqrt(seq_t)        */
#include "../utils/seq_alloc.h"           /* seq_alloc(size_t)        */

static seq_t max_square_exp_uint64(uint64_t n)
{
    if (n <= 1) return 0;

    /* Find sqrt(n) using provided integer sqrt */
    seq_t sq = floor_sqrt((seq_t)n);
    uint64_t limit = (uint64_t)sq + 1;   /* slightly exceed to cover odd sqrt cases */

    /* Generate primes up to limit */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);
    if (!primes) return 0; /* safety fall‑back */

    seq_t maxE = 0;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp > 0) {
            seq_t candidate = (seq_t)(2 * (exp / 2));
            if (candidate > maxE) maxE = candidate;
        }
    }

    /* Remaining prime factor > sqrt(original n) has exponent 1 -> contributes 0 */
    free(primes);
    return maxE;
}

seq_t a(idx_t n)
{
    return max_square_exp_uint64((uint64_t)n);
}
