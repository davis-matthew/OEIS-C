/*
 * A290107  a(1) = 1; for n > 1, a(n) = product of distinct exponents in the
 *          prime factorization of n.
 * https://oeis.org/A290107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of distinct exponents in the prime factorization
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1U)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    /* At most 64 distinct exponents for a 64‑bit integer */
    size_t exps[64];
    size_t cnt = 0;

    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            size_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            exps[cnt++] = e;
        }
    }
    /* Remaining prime factor larger than sqrt(m) */
    if (m > 1)
        exps[cnt++] = 1;

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        bool unique = true;
        for (size_t j = 0; j < i; ++j)
        {
            if (exps[j] == exps[i])
            {
                unique = false;
                break;
            }
        }
        if (unique)
            prod *= (seq_t)exps[i];
    }
    return prod;
}
