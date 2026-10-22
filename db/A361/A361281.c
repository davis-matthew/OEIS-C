/*
 * A361281  a(n) = n! * Sum_{k=0..n} binomial(n*k,n-k)/k!.
 * https://oeis.org/A361281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} binomial(n*k, n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                    /* a(0) = 1 */

    /* compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t sum = 0;
    seq_t fact_k = 1;                 /* current k! (starts at 0! = 1) */

    for (idx_t k = 1; k <= n; ++k)
    {
        fact_k *= (seq_t)k;                /* compute k! */
        seq_t bin = binomial(n * k, n - k);/* binomial(n*k, n-k) */
        seq_t factor = fact_n / fact_k;    /* n! / k!  (integer division) */
        seq_t term = bin * factor;         /* bin * (n! / k!) */
        sum += term;
    }
    /* k = 0 term is 0 for n > 0, so no need to add it */
    return sum;
}
