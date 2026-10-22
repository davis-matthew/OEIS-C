/*
 * A360861  a(n) = Sum_{k=0..n} binomial(n, ceiling(k/2)) * binomial(n,
 *          floor(k/2)).
 * https://oeis.org/A360861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(C(2*n+1,n)+C(n,floor(n/2))^2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute binomial coefficient C(n,k) in a way that avoids overflow of
   intermediate products for values that fit in seq_t. */
static seq_t binom_safe(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;      /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = n - k + i;     /* (n-k+i) */
        seq_t den = i;

        /* Reduce res with denominator */
        seq_t g = gcd(res, den);
        res /= g;
        den /= g;

        /* Reduce numerator with remaining denominator */
        g = gcd(num, den);
        num /= g;
        den /= g;                  /* den should now be 1 */

        res = res * num;          /* safe for values that fit */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t mid   = binom_safe(n, n / 2);             /* C(n,floor(n/2)) */
    seq_t midSq = mid * mid;                        /* square it */
    seq_t hb    = binom_safe(2*n + 1, n);           /* C(2*n+1,n)   */
    seq_t sum   = hb + midSq;
    return sum / 2;
}
