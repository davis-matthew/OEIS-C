/*
 * A372960  Concatenation of the exponents in the prime factorization of
 *          2*n-1.
 * https://oeis.org/A372960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A037916(2*n-1), i.e., concatenation of the exponents in
 *            the prime factorization of 2*n-1
 * Range:     u8 n <= 472, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: number of decimal digits in a uint64_t */
static inline size_t digits_of_uint64(uint64_t x)
{
    size_t d = 0;
    do
    {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* out of range, but safe */

    uint64_t m = (uint64_t)n * 2ULL - 1ULL;  /* 2*n-1 */

    /* Special case: m = 1 => no prime factors, answer 0 */
    if (m < 2ULL)
        return (seq_t)0;

    /* Exponents of prime factors, maximum 64 factors for 64‑bit numbers */
    uint64_t exponents[64];
    size_t expcnt = 0;

    uint64_t t = m;

    /* Handle factor 2 */
    if (t & 1ULL)
    {
        /* t even */
        size_t cnt = 0;
        while ((t & 1ULL) == 0ULL)
        {
            t >>= 1;
            ++cnt;
        }
        exponents[expcnt++] = (uint64_t)cnt;
    }

    /* Odd factors using 6k±1 loop */
    for (uint64_t p = 3; p * p <= t; p += 2)
    {
        if (t % p == 0)
        {
            size_t cnt = 0;
            while (t % p == 0)
            {
                t /= p;
                ++cnt;
            }
            exponents[expcnt++] = (uint64_t)cnt;
        }
    }

    /* Remaining prime > sqrt(t) */
    if (t > 1ULL)
        exponents[expcnt++] = 1ULL;

    seq_t res = 0;
    for (size_t i = 0; i < expcnt; ++i)
    {
        uint64_t e = exponents[i];
        seq_t pow10 = 1;
        uint64_t tmp = e;
        do
        {
            pow10 *= 10;
            tmp /= 10;
        } while (tmp);
        res = res * pow10 + (seq_t)e;
    }

    return res;
}
