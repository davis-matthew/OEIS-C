/*
 * A366555  G.f. A(x) satisfies A(x) = 1 + x + x^3*A(x)^3.
 * https://oeis.org/A366555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..floor(n/3)} binomial(2k+1,n-3k) *
 *            binomial(3k,k) / (2k+1)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 28, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t, idx_t) */

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t maxk = n / 3;                     /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* 3*k <= n by loop limit, so n-3*k is non‑negative */
        seq_t b1 = binomial((idx_t)(2 * k + 1), (idx_t)(n - 3 * k));
        if (b1 == (seq_t)0)            /* binomial may return 0 for invalid args */
            continue;
        seq_t b2   = binomial((idx_t)(3 * k), (idx_t)k);
        seq_t denom = (seq_t)(2 * k + 1);
        sum += (b1 * b2) / denom;     /* division is exact */
    }
    return sum;
}
