/*
 * A386542  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(3*n+2*k,k).
 * https://oeis.org/A386542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(3*n+2*k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum   = (seq_t)1;   /* k = 0 term: binom(n,0)*binom(3n,0) = 1 */

    seq_t bin_nk = (seq_t)1;   /* binom(n,0)        */
    seq_t bin_mk = (seq_t)1;   /* binom(3n+0,0)    */

    for (idx_t k = 1; k <= n; ++k) {
        /* update binom(n,k) */
        bin_nk = bin_nk * (seq_t)(n - k + 1) / (seq_t)k;

        /* update binom(3*n+2*k, k) by recurrence
           Let M = 3*n + 2*(k-1).  Then
           binom(3*n+2*k, k)
             = binom(3*n+2*(k-1), k-1) *
               (M+2)(M+1) /
               (k * (3*n + k))          */
        seq_t M   = (seq_t)(3 * n + 2 * (k - 1));
        seq_t num = (M + 1) * (M + 2);
        seq_t den = (seq_t)k * (3 * n + k);

        /* reduce fraction before multiplication to keep intermediates small */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(bin_mk, den);
        bin_mk /= g;
        den /= g;

        bin_mk = bin_mk * num / den;

        /* accumulate the term */
        sum += bin_nk * bin_mk;
    }

    return sum;
}
