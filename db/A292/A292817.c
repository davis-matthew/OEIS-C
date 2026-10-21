/*
 * A292817  b(0) = 1, b(2*n-1) =
 *          1/(1+1^2/(1+1^2/(1+2^2/(1+2^2/(...+(n-1)^2/(1+n^2)))))) and b(2*n)
 *          = 1/(1+1^2/(1+1^2/(1+2^2/(1+2^2/(...+n^2/(1+n^2)))))). a(n) is the
 *          denominator of b(n).
 * https://oeis.org/A292817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of b(n) where * b(0) = 1 and for n>0 * b(2*n)
 *            = 1/(1+1^2/(1+1^2/(1+2^2/(1+2^2/...+n^2/(1+n^2))))) * b(2*n-1) =
 *            1/(1+1^2/(1+1^2/(1+2^2/(1+2^2/...+(n-1)^2/(1+n^2))))) * This
 *            function computes a(n) for all n≥0 using only integer arithmetic
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* b(0) = 1 -> a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* k = ceil(n/2) = (n+1)/2 */
    idx_t k_idx = (n + 1) / 2;
    seq_t k = (seq_t)k_idx;

    seq_t num, den; /* current fraction cur = num/den */

    if ((n & 1) == 0) /* even index: 2*k */
    {
        seq_t w = (seq_t)1 + k * k;            /* 1 + k^2 */
        num = w + k * k;                      /* numerator of deep fraction 1 + k^2 / w */
        den = w;                               /* denominator of deep fraction */
    }
    else /* odd index: 2*k-1 */
    {
        num = (seq_t)1 + k * k;                /* 1 + k^2 */
        den = (seq_t)1;
    }

    /* Apply two nested levels for each i from k-1 down to 1 */
    for (idx_t i_idx = k_idx - 1; i_idx >= 1; --i_idx)
    {
        seq_t i = (seq_t)i_idx;
        seq_t i2 = i * i;                     /* i^2 */

        /* cur' = 1 + i2 / cur  -> (num + i2*den)/num */
        seq_t new_num = num + i2 * den;
        seq_t new_den = num;

        /* Reduce fraction */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;

        /* second level of the same i */
        new_num = num + i2 * den;
        new_den = num;
        g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* b(n) = 1/cur = den/num  -> a(n) = num */
    return num;
}
