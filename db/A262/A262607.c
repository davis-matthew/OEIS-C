/*
 * A262607  Sum_{k=0..n} ((k+1)*binomial(n+1,k)*binomial(2*n-k,n))/(n+1).
 * https://oeis.org/A262607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} ((k+1)*C(n+1,k)*C(2n-k,n))/(n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t k;
    seq_t sum = 0;

    /* precompute value n+1 and 2*n to avoid repeated evaluation */
    idx_t n1 = n + 1;
    idx_t twon = n + n;

    for (k = 0; k <= n; ++k)
    {
        seq_t A = binomial(n1, k);          /* C(n+1, k) */
        seq_t B = binomial(twon - k, n);   /* C(2n-k, n) */

        seq_t num   = (seq_t)(k + 1);      /* numerator factor (k+1) */
        seq_t denom = (seq_t)n1;           /* denominator (n+1) */

        /* Reduce fraction to avoid overflow */
        seq_t g = gcd(num, denom);
        num   /= g;
        denom /= g;

        g = gcd(A, denom);
        A   /= g;
        denom /= g;

        g = gcd(B, denom);
        B   /= g;
        denom /= g;

        /* After reductions denom should be 1; otherwise use 128‑bit arithmetic if available */
        seq_t term = 0;
        if (denom == 1)
        {
            /* All factors now fit without division */
            term = num * A * B;
        }
        else
        {
#if defined(__SIZEOF_INT128__)
            unsigned __int128 big = (unsigned __int128)num *
                                    (unsigned __int128)A *
                                    (unsigned __int128)B;
            term = (seq_t)(big / denom);
#else
            term = (num * A * B) / denom;
#endif
        }

        sum += term;
    }

    return sum;
}
