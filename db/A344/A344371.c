/*
 * A344371  a(n) = Sum_{k=1..n} (-1)^(n-k) gcd(k,n).
 * https://oeis.org/A344371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs(∑_{k=1..n} (-1)^(n-k) gcd(k,n))
 * Range:     u8 n <= 79, u16 n <= 5039, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A344371 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    long long sum = 0;
    seq_t sn = (seq_t)n;                 /* convert once for gcd */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t g = gcd((seq_t)k, sn);     /* gcd(k, n) */
        if (((n - k) & 1ULL) == 0)       /* (n-k) even  => + sign */
            sum += (long long)g;
        else
            sum -= (long long)g;         /* (n-k) odd  => - sign */
    }
    if (sum < 0)                          /* absolute value */
        sum = -sum;
    return (seq_t)sum;
}
