/*
 * A357960  a(n) = A005259(n-1)^5 * A005258(n)^6.
 * https://oeis.org/A357960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A005259(n-1)^5 * A005258(n)^6
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

/* Sum for A005258(n) = Σ_{k=0..n} C(n,k)^2 * C(n+k,k) */
static seq_t sum_A005258(idx_t n)
{
    seq_t binom = 1;   /* C(n,0) */
    seq_t B     = 1;   /* C(n,0) */
    seq_t sum   = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binom * binom * B;
        sum += term;

        if (k < n) {
            seq_t mult1 = binom * (seq_t)(n - k);
            seq_t div1  = (seq_t)(k + 1);
            binom = mult1 / div1;

            seq_t mult2 = B * (seq_t)(n + k + 1);
            seq_t div2  = (seq_t)(k + 1);
            B = mult2 / div2;
        }
    }
    return sum;
}

/* Sum for A005259(n) = Σ_{k=0..n} C(n,k)^2 * C(n+k,k)^2 */
static seq_t sum_A005259(idx_t n)
{
    seq_t binom = 1;   /* C(n,0) */
    seq_t B     = 1;   /* C(n,0) */
    seq_t sum   = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binom * binom * B * B;
        sum += term;

        if (k < n) {
            seq_t mult1 = binom * (seq_t)(n - k);
            seq_t div1  = (seq_t)(k + 1);
            binom = mult1 / div1;

            seq_t mult2 = B * (seq_t)(n + k + 1);
            seq_t div2  = (seq_t)(k + 1);
            B = mult2 / div2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t a258 = sum_A005258(n);     /* A005258(n)   */
    seq_t a259 = sum_A005259(n - 1); /* A005259(n-1) */

    seq_t p5 = ipow_idx(a259, 5);
    seq_t p6 = ipow_idx(a258, 6);

    return p5 * p6;
}
