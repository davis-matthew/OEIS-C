/*
 * A292629  a(n) = n! * [x^n] exp(n*x)*BesselI(1,2*x).
 * https://oeis.org/A292629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n-1)] (1 + n*x + x^2)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A292629 for any n >= 0, using only integer arithmetic. */
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Upper bound for the summation index t = 0 .. floor((n-1)/2) */
    idx_t max_t = (n - 1) / 2;

    seq_t res = 0;

    /* B = binom(n, t+1), start with t = 0: binom(n,1) = n */
    seq_t B = (seq_t)n;

    /* C = binom(n-1-t, t), start with t = 0: binom(n-1,0) = 1 */
    seq_t C = 1;

    /* P = n^(n-1-2t), start with t = 0 */
    seq_t P = big_pow((seq_t)n, (seq_t)(n - 1));

    /* Precompute constant n^2 for the P update */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    for (idx_t t = 0; t <= max_t; ++t)
    {
        /* term = B * C * P */
        seq_t term = B;
        term = term * C;
        term = term * P;
        res += term;

        if (t == max_t) break;

        /* Update B: B_{t+1} = B_t * (n - t - 1) / (t + 2) */
        B = B * ((seq_t)(n - t - 1));
        B = B / ((seq_t)(t + 2));

        /* Update C: C_{t+1} = C_t * (n - 1 - 2t) * (n - 2 - 2t)
                    / ((t + 1) * (n - 1 - t))   (integer division) */
        C = C * ((seq_t)(n - 1 - 2 * t));
        C = C * ((seq_t)(n - 2 - 2 * t));
        C = C / ((seq_t)(t + 1));
        C = C / ((seq_t)(n - 1 - t));

        /* Update P: P_{t+1} = P_t / n^2 */
        P = P / n_sq;
    }

    return res;
}
