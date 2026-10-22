/*
 * A371661  G.f. satisfies A(x) = 1 + x * A(x)^3 * (1 + A(x))^2.
 * https://oeis.org/A371661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{k=0..⌊(n-1)/2⌋} 4^(n-k) * binomial(n,k) *
 *            binomial(4n-k, n-1-2k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_int.h"    /* pow_int(seq_t base, idx_t exp) */
#include "../utils/binomial.h"  /* binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    idx_t Kmax = (n - 1) / 2;          /* upper bound for k */
    seq_t sum  = 0;

    /* binomial(n,0) = 1, 4^n */
    seq_t bin_nk = (seq_t)1;                 /* C(n,0)  */
    seq_t pow4   = pow_int((seq_t)4, n);     /* 4^n     */

    for (idx_t k = 0; k <= Kmax; ++k)
    {
        idx_t m     = n - 1 - 2 * k;         /* n-1-2k  */
        seq_t bin4  = binomial(4 * n - k, m); /* C(4n-k, m) */

        /* term = 4^(n-k) * C(n,k) * C(4n-k, n-1-2k) */
        seq_t term = pow4 * bin_nk;
        term       = term * bin4;

        sum += term;

        /* prepare for next k, except after final iteration */
        if (k < Kmax)
        {
            pow4   /= (seq_t)4;                           /* 4^(n-(k+1))  */
            bin_nk  = bin_nk * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }

    return sum / (seq_t)n;
}
