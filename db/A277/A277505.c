/*
 * A277505  E.g.f.: -LambertW(-x)/(1-x).
 * https://oeis.org/A277505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n≥0, a(n+1) = (n+1)*a(n) + (n+1)^n. This recurrence
 *            is equivalent to the sum definition: a(n) = Σ_{k=1..n}
 *            binomial(n,k) * k^(k-1) * (n-k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 355 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"

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

/* Compute the n‑th term of OEIS A277505. */
seq_t a(idx_t n)
{
    seq_t result = 0;                /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t pow_term = ipow((seq_t)i, (seq_t)(i - 1));
        result = mul_small(result, (unsigned int)i) + pow_term;
    }
    return result;
}
