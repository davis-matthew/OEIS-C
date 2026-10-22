/*
 * A322079  a(n) = n^2 * Sum_{ p^k | n } k / p^2, where p are primes dividing
 *          n with multiplicity k.
 * https://oeis.org/A322079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * Sum_{p^k|n} k/p^2
 * Range:     u8 n <= 15, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t nn = (uint64_t)n;
    seq_t n2 = (seq_t)nn * (seq_t)nn;          /* n^2 */
    seq_t sum = 0;

    uint64_t temp = nn;

    /* trial division */
    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            unsigned int k = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++k;
            }
            seq_t term = (n2 / ((seq_t)p * (seq_t)p)) * (seq_t)k;
            sum += term;
        }
    }

    /* remaining prime factor > sqrt(nn) */
    if (temp > 1)
    {
        uint64_t p = temp;
        seq_t term = (n2 / ((seq_t)p * (seq_t)p)) * (seq_t)1;
        sum += term;
    }

    return sum;
}
