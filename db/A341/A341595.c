/*
 * A341595  Number of strictly superior squarefree divisors of n.
 * https://oeis.org/A341595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1564, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A341595: Number of strictly superior squarefree divisors of n.
   The function enumerates all squarefree divisors of n by listing distinct
   prime factors, then counts those greater than ⌊√n⌋.  */
#include <stdint.h>
#include <stddef.h>

static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n, mid;
    while (lo <= hi)
    {
        mid = (lo + hi) >> 1;
        if (mid * mid <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    uint64_t num = (uint64_t)n;
    uint64_t tmp = num;

    /* collect distinct prime factors */
    int primes[64];
    size_t p_cnt = 0;

    if ((tmp & 1) == 0)
    {
        primes[p_cnt++] = 2;
        while ((tmp & 1) == 0)
            tmp >>= 1;
    }
    for (uint64_t d = 3; d * d <= tmp; d += 2)
    {
        if (tmp % d == 0)
        {
            primes[p_cnt++] = (int)d;
            while (tmp % d == 0)
                tmp /= d;
        }
    }
    if (tmp > 1)
        primes[p_cnt++] = (int)tmp;

    /* compute floor sqrt of n */
    seq_t sqrt_n = (seq_t)isqrt_u64(num);

    /* enumerate all squarefree divisors */
    size_t subsets = (size_t)1 << p_cnt;
    size_t count = 0;

    for (size_t mask = 0; mask < subsets; ++mask)
    {
        seq_t d = 1;
        for (size_t i = 0; i < p_cnt; ++i)
            if (mask & ((size_t)1 << i))
                d *= (seq_t)primes[i];
        if (d > sqrt_n)
            ++count;
    }

    return (seq_t)count;
}
