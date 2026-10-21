/*
 * A261682  a(n) = 2^n+(1+(n mod 2)/2)*C(n+1,floor((n+(n mod 2))/2))-1.
 * https://oeis.org/A261682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + (1 + (n mod 2)/2) * C(n+1, floor((n+(n mod 2))/2))
 *            - 1 a(0) = 1, a(1) = 4, a(2) = 6, … (OEIS A261682)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow2.h"

/* Multiplicative binomial coefficient: C(n,k) for 0 ≤ k ≤ n */
static seq_t binomial_seq(idx_t n, idx_t k)
{
    if (k > n)         return 0;
    if (k == 0 || k == n) return 1;

    /* Take advantage of symmetry */
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t numerator   = (seq_t)(n + 1 - i);   /* (n+1-i) */
        seq_t denominator = (seq_t)i;
        seq_t g = gcd(numerator, denominator);     /* cancel common factors */
        numerator   /= g;
        denominator  /= g;

        g = gcd(res, denominator);
        res        /= g;
        denominator /= g;          /* should be 1 now */

        /* multiplication cannot overflow: result stays integers <= C(n,k) */
        res *= numerator;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t pow2_n = pow2(n);                 /* 2^n  */
    if ((n & 1) == 0)                      /* even n */
    {
        seq_t binom = binomial_seq(n + 1, n / 2);   /* C(n+1, n/2) */
        return pow2_n + binom - 1;
    }
    else                                   /* odd n */
    {
        seq_t binom = binomial_seq(n + 1, (n + 1) / 2); /* C(n+1, (n+1)/2) */
        seq_t temp  = binom * 3;                 /* (3 * binom) */
        temp /= 2;                               /* divide by 2 exactly */
        return pow2_n + temp - 1;
    }
}
