/*
 * A380322  a(n) is the sum of exponentially odd divisors of n^2.
 * https://oeis.org/A380322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of exponentially odd divisors of n^2. a(p^e) =
 *            (p^(2*e+1) - p)/(p^2-1) + 1 a(n) is multiplicative
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 3757, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9065 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

/* The function a computes the A380322 sequence value for index n (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; not used but safe */
    uint64_t nn = (uint64_t)n;

    /* At most 64 distinct primes; that is more than enough for n <= UINT64_MAX. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = (uint64_t)exps[i];

        /* Compute (p^(2*e+1) - p) / (p^2-1) + 1 */
        seq_t num = ipow_u64_idx(p, (idx_t)(2ULL * e + 1)) - (seq_t)p;
        seq_t den = (seq_t)(p * p - 1ULL);
        seq_t term = (num / den) + (seq_t)1;
        result *= term;
    }
    return result;
}
