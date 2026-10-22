/*
 * A386865  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(2*n+2,k) *
 *          binomial(2*n-k-1,n-k).
 * https://oeis.org/A386865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * binomial(2*n+2,k) *
 *            binomial(2*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t i;
    seq_t sum  = 0;
    seq_t pow  = pow2(n);                     /* 2^n   */
    seq_t c1   = 1;                            /* C(2n+2,0) */
    seq_t c2   = binomial(2*n - 1, n);       /* C(2n-1,n) */

    seq_t two = (seq_t)2;

    for (i = 0; i <= n; ++i) {
        seq_t term = pow * c1 * c2;
        sum += term;
        if (i == n) break;                   /* avoid updating beyond last term */

        /* update for next k = i+1 */
        pow /= two;

        seq_t num1 = (seq_t)(2*n + 2 - i);
        seq_t den1 = (seq_t)(i + 1);
        c1 = c1 * num1 / den1;

        seq_t num2 = (seq_t)(n - i);
        seq_t den2 = (seq_t)(2*n - i - 1);
        c2 = c2 * num2 / den2;
    }
    return sum;
}
