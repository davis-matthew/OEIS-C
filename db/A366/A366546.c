/*
 * A366546  Expansion of e.g.f. -log(1 - x^3 * exp(x)).
 * https://oeis.org/A366546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..floor(n/3)} k^{n-3k-1}/(n-3k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    /* maximum k such that 3k <= n */
    idx_t max_k = n / 3;
    seq_t sum = (seq_t)0;

    /* initial falling product for k = 1 : n * (n-1) * (n-2) */
    seq_t fall = (seq_t)n * (seq_t)(n-1) * (seq_t)(n-2);

    for (idx_t k = 1; k <= max_k; ++k)
    {
        idx_t t = n - 3 * k;              /* t = n - 3k */
        seq_t term;

        if (t == 0)                      /* exponent is -1 -> divide by k */
        {
            term = fall / (seq_t)k;
        }
        else                              /* exponent is t-1 >= 0 */
        {
            seq_t k_pow = big_pow((seq_t)k, (seq_t)(t - 1));
            term = fall * k_pow;
        }

        sum += term;

        /* Update falling product for the next k (if any) */
        if (k < max_k)
        {
            for (idx_t j = 0; j < 3; ++j)
            {
                fall *= (seq_t)(n - 3 * k - j);
            }
        }
    }

    return sum;
}
