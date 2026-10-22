/*
 * A373521  Expansion of e.g.f. exp(x/(1 - x^4)^(3/4)).
 * https://oeis.org/A373521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/4⌋} binomial(3*n/4-2*k-1,k)/(n-4*k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static void muldiv(seq_t *x, seq_t mul, seq_t div)
{
    seq_t g = gcd(mul, div);
    mul /= g;
    div /= g;
    seq_t g2 = gcd(*x, div);
    *x /= g2;
    div /= g2;
    *x *= mul;
    if (div != 0)
        *x /= div;
}

seq_t a(idx_t n)
{
    // K = floor(n/4)
    idx_t K = n / 4;
    seq_t result = 0;
    seq_t ratio = 1;   /* ratio = n!/(n-4k)! for current k */

    for (idx_t k = 0; k <= K; ++k)
    {
        /* term = ratio * Prod_{j=0}^{k-1} (t_num-4j) / (4*(j+1)) */
        seq_t term = ratio;
        seq_t t_num = (seq_t)(3 * n - 8 * k - 4);

        for (idx_t j = 0; j < k; ++j)
        {
            seq_t mul = t_num - (seq_t)(4 * j);
            seq_t div = (seq_t)(4 * (j + 1));
            muldiv(&term, mul, div);
        }

        result += term;

        if (k < K)
        {
            /* multiply ratio by the next four factors (n-4k-3)...(n-4k) */
            seq_t m1 = (seq_t)(n - 4 * k - 3);
            seq_t m2 = (seq_t)(n - 4 * k - 2);
            seq_t m3 = (seq_t)(n - 4 * k - 1);
            seq_t m4 = (seq_t)(n - 4 * k);
            ratio *= m1;
            ratio *= m2;
            ratio *= m3;
            ratio *= m4;
        }
    }
    return result;
}
