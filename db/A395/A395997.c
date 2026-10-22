/*
 * A395997  Numbers k > 3 such that the greedy Egyptian fraction
 *          representation of 3/k has more terms than the shortest
 *          representation of 3/k as a sum of unit fractions.
 * https://oeis.org/A395997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 5040, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th term of OEIS A395997.
   The terms are all integers k>3 that satisfy:
     * k is odd and not divisible by 3  →  k ≡ 1 (mod 6);
     * k has at least one prime divisor p ≡ 2 (mod 3).
   For such k the greedy Egyptian fraction decomposition of 3/k
   uses more unit fractions than the shortest possible decomposition. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static bool has_mod2_prime_factor(uint64_t n)
{
    /* factor_u64 will write prime factors into primes[] and their
       exponents into exps[].  cnt is output length.          */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] % 3ULL == 2ULL)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (uint64_t k = 5; ; ++k) {
        if ((k % 6ULL) != 1ULL)        /* must be ≡1 (mod 6) */
            continue;
        if (!has_mod2_prime_factor(k))
            continue;
        ++found;
        if (found == n)
            return (seq_t)k;
    }
}
