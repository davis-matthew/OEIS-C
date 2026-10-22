/*
 * A361777  Expansion of e.g.f. A(x) satisfying A(x) = exp( x * A(x)^x ).
 * https://oeis.org/A361777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-2*k) * (n-k)^(k-1) * n! /
 *            ((n-2*k)! * k!) for n>0, with a(0)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* for seq_t big_pow(seq_t base, seq_t exp) */

/* Computes the n-th term of OEIS A361777 */
seq_t a(idx_t n)
{
    if (n == 0)                       /* a(0) = 1 */
        return (seq_t)1;

    seq_t sum = (seq_t)1;             /* term for k = 0 */
    seq_t R = (seq_t)1;               /* R_0 = n! / ((n-0)! * 0!) = 1 */

    for (idx_t k = 1; k <= n / 2; ++k) {
        /* Update R: R_k = R_{k-1} * (n-2(k-1)) * (n-2(k-1)-1) / k   */
        seq_t mult1 = (seq_t)(n - 2 * (k - 1));
        seq_t mult2 = (seq_t)(n - 2 * (k - 1) - 1);
        seq_t numer  = R * mult1;
        numer = numer * mult2;
        R = numer / k;                /* division is exact */

        /* Compute (n-k)^(k-1) */
        seq_t base  = (seq_t)(n - k);
        seq_t exp   = (seq_t)(k - 1);
        seq_t powv  = big_pow(base, exp);   /* 0 exponent gives 1 */

        /* Term contribution: (n-2k) * (n-k)^(k-1) * R_k */
        seq_t term = (seq_t)(n - 2 * k);
        term = term * powv;
        term = term * R;

        sum += term;
    }
    return sum;
}
