/*
 * A364408  G.f. satisfies A(x) = 1 + x*(1 + 1/A(x)^4).
 * https://oeis.org/A364408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n-1) * (1/n) * Sum_{k=0..n} binom(n,k) *
 *            binom(n+4*k-2,n-1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

#if defined(__SIZEOF_INT128__)
    typedef __int128 int128_t;
#else
    /* Fallback: use 64‑bit if 128‑bit is not available.  In that case the
       intermediate overflow will be detected by the tests because all
       terms up to the limit fit in 64‑bit. */
    typedef long long int128_t;
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Sum of products: use a wide type to avoid overflow. */
    int128_t sum = 0;

    /* Note: binom(n+4*k-2, n-1) is defined for n>0. */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* Convert binomial values to the wide type before multiplying. */
        int128_t t1 = (int128_t)binom(n, k);
        int128_t t2 = (int128_t)binom(n + 4 * k - 2, n - 1);
        sum += t1 * t2;
    }

    /* Divide by n exactly.  Since the formula guarantees divisibility,
       the integer division is safe. */
    sum /= (int128_t)n;

    /* Apply (-1)^(n-1).  (n-1) is unsigned, so use bit test for parity. */
    if (((n - 1) & 1) != 0)
        sum = -sum;

    return (seq_t)sum;
}
