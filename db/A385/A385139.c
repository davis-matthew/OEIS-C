/*
 * A385139  The sum of divisors d of n such that n/d has exponents in its
 *          prime factorization that are all powers of 2 (A138302).
 * https://oeis.org/A385139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} d for which n/d has prime exponents that are
 *            powers of 2 (equivalently, a(n) is multiplicative with a(p^e) =
 *            p^e + Σ_{k=0..⌊log2 e⌋} p^{e-2^k} )
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9314 (2026-11-01)
 * Helpers:   factor_u64, ilog2, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ilog2.h"
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case and indices below the offset */
    if (n < 1)
        return (seq_t)0;

    uint64_t u = (uint64_t)n;
    if (u == 1)
        return (seq_t)1;

    /* Temporary storage for prime factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t factor_cnt = 0;
    factor_u64(u, primes, exps, &factor_cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* term = p^e + Σ p^{e-2^k} */
        seq_t term = ipow_u64_idx(p, e);
        uint64_t maxk = ilog2(e);
        for (uint64_t k = 0; k <= maxk; ++k) {
            uint64_t shift = 1ULL << k;
            /* e >= shift is guaranteed by the loop range */
            term += ipow_u64_idx(p, e - shift);
        }

        /* Multiply into the running product */
        result = seq_mul(result, term);
    }
    return result;
}
