/*
 * A317277  a(n) = Sum_{k=0..n} binomial(n-1,k-1)*k^n*n!/k!; a(0) = 1.
 * https://oeis.org/A317277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n-1,k-1)*k^n*n!/k! with a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Working variables */
    seq_t sum  = 0;      /* accumulated result                              */
    seq_t B    = 1;      /* current binomial C(n-1, k-1)                     */
    seq_t P    = 1;      /* product of integers from k+1 to n               */

    for (idx_t k = n; k >= 1; --k) {
        /* k^n */
        seq_t pow_kn = big_pow((seq_t)k, (seq_t)n);

        /* term = B * k^n * P */
        seq_t term = B * pow_kn;
        term *= P;

        sum += term;

        /* Update P for next lower k: P_new = k * P   (product from k to n) */
        if (k > 1)
            P *= (seq_t)k;

        /* Update B for next lower k:
           C(n-1, k-2) = C(n-1, k-1) * (k-1) / (n - k + 1)               */
        if (k > 1) {
            idx_t numer   = k - 1;                /* <= n-1              */
            idx_t denom   = n - k + 1;            /* <= n                */
            B = (B * (seq_t)numer) / (seq_t)denom;
        }
    }

    return sum;
}
