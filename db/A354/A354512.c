/*
 * A354512  Number of solutions m >= 2 to m - gpf(m) = n, gpf = A006530.
 * https://oeis.org/A354512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of distinct prime factors p of n such that gpf(n+p)=p.
 *            a(n) = |{ p prime | p | n, gpf(n+p)=p }| with offset 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n < 1) /* defensive: the sequence is undefined for 0 */
        return 0;

    uint64_t t = (uint64_t)n;            /* work with a 64‑bit integer */
    uint64_t primes[64];
    size_t   cnt = 0;

    /* factorize t, recording each distinct prime factor */
    uint64_t temp = t;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            primes[cnt++] = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        primes[cnt++] = temp;

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p   = primes[i];
        uint64_t sum = t + p;
        if (gpf(sum) == p)
            result++;
    }

    return result;
}
