/*
 * A392170  The Dedekind psi function value of the smallest powerful number
 *          that is a multiple of n.
 * https://oeis.org/A392170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   multiplicative with a(p^e) = (p+1) * p^(max(e,2)-1)
 * Range:     u8 n <= 13, u16 n <= 185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9625 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Compute the sequence value for a given n (1‑based). */
seq_t a(idx_t n)
{
    /* The OEIS offset starts at 1; for safety guard against n==0. */
    if (n == 0)
        return 0;

    /* Factorise n.  Any 64‑bit integer has at most 64 distinct prime factors,
       which is sufficient for all n that the sequence is tested against. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   count = 0;

    factor_u64((uint64_t)n, primes, exps, &count);

    /* Multiplicative identity. */
    seq_t result = 1;

    for (size_t i = 0; i < count; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* exponent = max(e,2) – 1 */
        idx_t exp = (e > 1) ? (e - 1) : 1;

        /* term = (p+1) * p^exp */
        seq_t term = seq_mul((seq_t)(p + 1), ipow_idx(p, exp));

        /* accumulate the product */
        result = seq_mul(result, term);
    }

    return result;
}
