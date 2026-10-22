/*
 * A351564  a(n) = 1 if all the exponents in the prime factorization of n are
 *          distinct, and 0 otherwise.
 * https://oeis.org/A351564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if all exponents in the prime factorization of n are
 *            distinct, 0 otherwise
 * Range:     u8 n <= 9822, u16 n <= 9822, u32 n <= 9822, u64 n <= 9822,
 *            u128 n <= 9822, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t rem = (uint64_t)n;
    uint32_t seen = 0;        /* bit e set if exponent e has been seen */

    for (uint64_t p = 2; p <= rem / p; ++p)
    {
        if (rem % p == 0)
        {
            uint32_t e = 0;
            while (rem % p == 0)
            {
                ++e;
                rem /= p;
            }
            if (seen & (1u << e)) return (seq_t)0; /* duplicate exponent */
            seen |= (1u << e);
        }
    }

    /* rem > 1 means a remaining prime factor with exponent 1 */
    if (rem > 1)
    {
        if (seen & (1u << 1)) return (seq_t)0;
        seen |= (1u << 1);
    }

    return (seq_t)1;
}
