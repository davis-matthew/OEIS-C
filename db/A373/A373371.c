/*
 * A373371  a(n) = 1 if the sum of prime factors with repetition is a multiple
 *          of 3, otherwise 0.
 * https://oeis.org/A373371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A001414(n) == 0 (mod 3)]
 * Range:     u8 n <= 1318, u16 n <= 9109, u32 n <= 9109, u64 n <= 9109,
 *            u128 n <= 9109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if the sum of prime factors of n (with multiplicity) is a multiple of 3, otherwise 0. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* We do not need to handle n < 1; the offset is 1.  */
    if (n < 1) return 0;

    /* Convert to a plain unsigned integer for factoring.  */
    uint64_t m = (uint64_t)n;

    seq_t sum = 0;

    /* Trial division to factor m and sum its prime factors.  */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        while (m % p == 0)
        {
            sum += (seq_t)p;
            m /= p;
        }
    }
    /* If any prime factor remains (m > 1), add it.  */
    if (m > 1)
        sum += (seq_t)m;

    /* Return 1 if the sum is divisible by 3, otherwise 0.  */
    return (seq_t)((sum % 3) == 0 ? 1 : 0);
}
