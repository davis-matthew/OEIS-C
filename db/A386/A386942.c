/*
 * A386942  a(n) = Sum_{k=0..n} (2*k+1) * binomial(2*k,k) *
 *          binomial(2*n-k,n-k).
 * https://oeis.org/A386942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (2*k+1) * binomial(2*k,k) *
 *            binomial(2*n-k,n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* k = 0 one‑time values */
    seq_t C1 = 1;                             /* binomial(0,0)            */
    seq_t C2 = binomial(2 * n, n);            /* binomial(2n,n)           */
    seq_t two_k_plus1 = 1;                    /* 2*0 + 1                  */
    seq_t sum = two_k_plus1 * C1 * C2;         /* term for k = 0           */

    for (idx_t k = 1; k <= n; ++k) {
        /* update binomial(2*k,k)   C1_k = C1_{k-1} * (4*k-2) / k   */
        seq_t mul1 = C1 * ((seq_t)(4 * k - 2));
        C1 = mul1 / (seq_t)k;

        /* update binomial(2*n-k,n-k)   C2_k = C2_{k-1} * (n-k+1) / (2*n-k+1) */
        seq_t mul2 = C2 * ((seq_t)(n - k + 1));
        C2 = mul2 / ((seq_t)(2 * n - k + 1));

        two_k_plus1 += 2;                     /* 2k+1 for current k       */

        sum += two_k_plus1 * C1 * C2;          /* accumulate term          */
    }

    return sum;
}
