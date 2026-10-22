/*
 * A325765  Number of integer partitions of n with a unique consecutive
 *          subsequence summing to every positive integer from 1 to n.
 * https://oeis.org/A325765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1; for n > 0, a(n) = d(n+1) - 1, where d(m) counts the
 *            positive divisors of m
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t divisor_count(idx_t m)
{
    seq_t total = (seq_t)1;
    idx_t temp = m;

    for (idx_t p = (idx_t)2; p * p <= temp; ++p)
    {
        if (temp % p == (idx_t)0)
        {
            int exp = 0;
            while (temp % p == (idx_t)0)
            {
                temp /= p;
                ++exp;
            }
            total = total * (seq_t)(exp + (seq_t)1);
        }
    }

    if (temp > (idx_t)1)
        total = total * (seq_t)2;

    return total;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;               /* a(0) = 1 */

    idx_t m = n + (idx_t)1;             /* evaluate divisor count for n+1 */
    seq_t d = divisor_count(m);        /* d(n+1) */
    return d - (seq_t)1;                /* exclude the number itself */
}
