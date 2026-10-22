/*
 * A317848  Multiplicative with a(p^e) = binomial(2*e, e).
 * https://oeis.org/A317848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1023, u32 n <= 9532, u64 n <= 9532,
 *            u128 n <= 9532, big any n
 * Helpers:   binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative function: a(1) = 1; for a prime power p^e, a(p^e) = binomial(2*e, e). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/binomial.h"

/* Computes the n-th term of sequence A317848.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Base case: a(1) = 1. */
    if (n <= 1)
        return (seq_t)1;

    /* Factor n; primes and exponents are stored in these arrays. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Accumulate the product of binomial(2*e, e) for each prime exponent. */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e     = (idx_t)exps[i];
        idx_t twoe  = e + e;            /* 2*e */
        prod *= binomial(twoe, e);
    }
    return prod;
}
