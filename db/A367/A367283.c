/*
 * A367283  G.f. satisfies A(x) = 1 + x*A(x)^2 * (1 + x*A(x)^3)^2.
 * https://oeis.org/A367283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binom(3*n - k + 1, k) * binom(2*k, n - k) /
 *            (3*n - k + 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t B     = 1;                /* B_k = binom(3*n - k + 1, k)  */
    idx_t two_n = 3 * n;            /* 3*n as idx_t for reuse             */

    for (idx_t k = 0; k <= n; ++k) {
        /* D_k = binom(2*k, n-k)   */
        idx_t two_k    = 2 * k;
        idx_t n_minus_k = n - k;
        seq_t D = binom(two_k, n_minus_k);

        if (D != 0) {
            /* A_k = 3*n - k + 1   */
            seq_t A = (seq_t)(two_n - k + 1);
            seq_t term = (B * D) / A;   /* division is exact */
            sum += term;
        }

        /* update B for next k+1 (if any) */
        if (k < n) {
            /* B_{k+1} = B_k * (3n - 2k + 1) * (3n - 2k) / ((3n - k + 1) * (k+1)) */
            idx_t num1  = two_n - 2 * k + 1;
            idx_t num2  = two_n - 2 * k;
            idx_t den1  = two_n - k + 1;
            idx_t den2  = k + 1;

            B = B * (seq_t)num1 / (seq_t)den1;   /* intermediate division is exact */
            B = B * (seq_t)num2 / (seq_t)den2;
        }
    }

    return sum;
}
