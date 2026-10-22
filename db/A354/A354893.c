/*
 * A354893  a(n) = n! * Sum_{d|n} d^(n - d) / (n/d)!.
 * https://oeis.org/A354893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d | n} d^(n - d) / (n/d)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

static inline seq_t product_range(idx_t low, idx_t high)
{
    seq_t res = 1;
    if (low > high) return res;      /* empty product */
    for (idx_t i = low; i <= high; ++i)
        res *= (seq_t)i;
    return res;
}

static inline seq_t term_for(idx_t n, idx_t d)
{
    idx_t nd = n / d;                                       /* n/d */
    seq_t ratio = product_range(nd + 1, n);                 /* n! / (n/d)! */
    seq_t pow_part = big_pow((seq_t)d, (seq_t)(n - d));      /* d^(n-d) */
    return ratio * pow_part;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            sum += term_for(n, d);
            if (d != n / d)
                sum += term_for(n, n / d);
        }
    }
    return sum;
}
