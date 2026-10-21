/*
 * A291047  Number of ways to write 4*n+1 as p^2 + q^2 + 8*r^2, where p is
 *          prime, and q and r are nonnegative integers.
 * https://oeis.org/A291047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of 4*n+1 as p^2 + q^2 + 8*r^2
 *            with p prime and q,r ≥ 0
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for a 64‑bit unsigned value */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

/* primality test for a 64‑bit unsigned value */
static inline bool is_prime_uint64(uint64_t m)
{
    if (m < 2)                 return false;
    if (m == 2 || m == 3)      return true;
    if (m % 2 == 0 || m % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= m; i += 6)
    {
        if (m % i == 0 || m % (i + 2) == 0) return false;
    }
    return true;
}

/* checks whether a 64‑bit unsigned integer is a perfect square */
static inline bool is_square_uint64(uint64_t x)
{
    uint64_t r = isqrt_uint64(x);
    return r * r == x;
}

/* OEIS sequence A291047 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* offset starts at 1 */

    seq_t target = (seq_t)n * 4 + 1;        /* 4*n+1 */

    seq_t count = 0;

    uint64_t maxp = isqrt_uint64((uint64_t)target);
    for (uint64_t p = 2; p <= maxp; ++p)
    {
        if (!is_prime_uint64(p)) continue;
        uint64_t p2 = p * p;
        uint64_t rem = (uint64_t)target - p2;

        uint64_t maxr = isqrt_uint64(rem / 8);
        for (uint64_t r = 0; r <= maxr; ++r)
        {
            uint64_t rr = r * r;
            uint64_t val = rem - 8 * rr;
            if (is_square_uint64(val))
                ++count;                 /* one valid representation */
        }
    }

    return count;
}
