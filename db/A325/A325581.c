/*
 * A325581  G.f.: Sum_{n>=0} (n+1) * x^n * (1+x)^(n*(n+1)).
 * https://oeis.org/A325581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * binomial(k*(k+1), n-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* The routine below is valid both for C11 and C++17.
   seq_t and idx_t are defined by the build system, so we
   do not re‑define them here.                                    */
seq_t a(idx_t n)
{
    /* Accumulated result for a(n).                                */
    seq_t total = 0;

    /* Outer sum over k.                                           */
    for (idx_t k = 0; k <= n; ++k) {
        /* m = k*(k+1)  (fits in seq_t by the terms of the series). */
        seq_t mk = (seq_t)k * (k + 1);

        /* r = n - k.  If r exceeds mk, the binomial is zero.       */
        idx_t r = n - k;
        if (r > mk)
            continue;                     /* contributes nothing */

        /* Compute binomial(mk, r) multiplicatively.                 */
        seq_t binom = 1;
#ifdef __cplusplus
        /* In C++ (including arbitrary‑precision builds), simply
           use the overloaded operators.                            */
        for (idx_t i = 1; i <= r; ++i)
            binom = binom * (mk - i + 1) / i;
        /* Accumulate the term (k+1)*binom.                         */
        total += (k + 1) * binom;
#else
        /* In C, use unsigned __int128 for intermediate products
           to avoid overflow for the common 64‑bit builds.         */
        for (idx_t i = 1; i <= r; ++i) {
            binom = (seq_t)(((__uint128_t)binom * (mk - i + 1)) / i);
        }
        total = (seq_t)(((__uint128_t)total + (__uint128_t)(k + 1) * binom));
#endif
    }

    return total;
}
