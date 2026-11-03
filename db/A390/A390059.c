/*
 * A390059  E.g.f. A(x) satisfies A(x) = exp( x * (1+x^3) * A(x)^2 ).
 * https://oeis.org/A390059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{k=0..⌊n/4⌋} (2*(n-3k)+1)^(n-3k-1) *
 *            binomial(n-3k, k) / (n-3k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 292 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;                   /* a(0) = 1 */
    }

    seq_t sum = 0;                           /* running sum of terms */
    idx_t maxk = n / 4;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 3 * k;                /* m = n - 3k, always >= 0 */

        /* Compute n! / m! as product of integers (m+1 .. n) */
        seq_t prod = 1;
        for (idx_t t = m + 1; t <= n; ++t) {
            prod = mul_small(prod, (unsigned int)t);
        }

        /* (2*m + 1)^(m-1) */
        seq_t base = 2 * m + 1;
        seq_t pow_term = ipow_i32(base, (int)(m - 1));

        /* binomial coefficient */
        seq_t bin = binomial(m, k);

        /* term = prod * pow_term * bin */
        seq_t term = seq_mul(prod, pow_term);
        term = seq_mul(term, bin);

        sum += term;          /* seq_t supports the + operator */
    }

    return sum;
}
