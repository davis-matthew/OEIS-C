/*
 * A333753  Sum of prime power divisors of n that are <= sqrt(n).
 * https://oeis.org/A333753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of prime power divisors of n that are <= sqrt(n) */

#include <stdint.h>

/* Integer square‑root for 64 bit values (floor sqrt).  */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 1) <= n)
        r++;
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t sqrt_n = isqrt_u64(n);
    idx_t temp   = n;
    seq_t sum    = 0;

    for (idx_t p = 2; p <= sqrt_n && temp > 1; ++p)
    {
        if (temp % p != 0)
            continue;

        /* count exponent of prime p */
        idx_t e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
        }

        /* add all powers p^k (k=1..e) that do not exceed sqrt(n) */
        seq_t power = (seq_t)p;          /* p^1 */
        for (idx_t k = 1; k <= e; ++k)
        {
            if (power <= (seq_t)sqrt_n)
                sum += power;
            /* next power */
            if (k < e)
                power *= (seq_t)p;
        }
    }

    return sum;
}
