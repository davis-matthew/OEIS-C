/*
 * A351271  Sum of the 8th powers of the squarefree divisors of n.
 * https://oeis.org/A351271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d^8 * mu(d)^2 = product_{p|n} (1 + p^8)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 16, u64 n <= 256,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 1 is the neutral element for multiplication, but we special‑case
       n == 1, in which case the only divisor is 1 and 1^8 = 1. */
    if (n == 1)
        return 1;

    /* Store distinct prime factors of n.  The number of distinct prime
       factors of a 64‑bit integer is at most 16, so 64 slots are more
       than enough. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        /* Compute p^8 as a seq_t value. */
        seq_t p8 = big_pow((seq_t)pr[i], (seq_t)8);
        /* Multiply the running product by (1 + p^8). */
        res = res * (1 + p8);
    }
    return res;
}
