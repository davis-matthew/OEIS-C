/*
 * A343939  Number of n-chains of divisors of n.
 * https://oeis.org/A343939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p^e∥n} C(n+e, e)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/factor_u64.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* factorize n; 64-bit numbers have at most 64 distinct prime factors */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;
    idx_t N = n;                 /* original n */
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = (idx_t)exps[i];
        seq_t bin = binomial(N + e, e);     /* C(N+e, e) = C(N+e, N) */
        prod = seq_mul(prod, bin);
    }
    return prod;
}
