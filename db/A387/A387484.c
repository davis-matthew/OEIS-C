/*
 * A387484  a(n) = Sum_{k=0..floor(n/3)} 2^(n-k) * binomial(k,n-3*k)^2.
 * https://oeis.org/A387484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^(n-k) * binomial(k, n-3*k)^2
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 27, u64 n <= 53, u128 n <= 103,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */
#include "../utils/pow2.h"        /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 3;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t r = n - 3 * k;
        if (r > k) continue;               /* binomial(k,r)=0 when r>k */
        seq_t b     = binomial(k, r);      /* b = C(k, r)       */
        seq_t b2    = b * b;                /* b^2               */
        seq_t pow    = pow2(n - k);         /* 2^(n-k)           */
        seq_t term   = b2 * pow;            /* b^2 * 2^(n-k)     */
        sum += term;                        /* accumulate result */
    }
    return sum;
}
