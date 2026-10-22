/*
 * A359466  a(n) = 1 if n has exactly one non-unitary prime factor, otherwise
 *          0.
 * https://oeis.org/A359466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n has exactly one non‑unitary prime factor]
 * Range:     u8 n <= 9844, u16 n <= 9844, u32 n <= 9844, u64 n <= 9844,
 *            u128 n <= 9844, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t m = (uint64_t)n;           /* work with 64‑bit integer */
    unsigned nonunit_count = 0;

    /* trial division up to sqrt(m) – safe against overflow using m/p */
    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            unsigned exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            if (exp > 1)
            {
                ++nonunit_count;
                if (nonunit_count > 1)
                    break;                 /* more than one non‑unitary factor */
            }
        }
    }
    /* any remaining factor m > 1 is a prime with exponent 1 – no effect */

    return (seq_t)(nonunit_count == 1);
}
