/*
 * A397224  Number of ordered pairs (i,j) such that i*j=n and |i-j| is a
 *          perfect square.
 * https://oeis.org/A397224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # {(i,j) | i·j=n and |i-j| is a perfect square}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses an integer square‑root routine for 64‑bit values. */
#include <stdbool.h>

static inline idx_t isqrt_u64(idx_t x)
{
    /* Fast integer square root for 64‑bit integers */
    if (x < 2) return x;
    idx_t r = 0;
    idx_t bit = 1ULL << 62;   /* highest power of 4 <= x */
    while (bit > x) bit >>= 2;
    while (bit)
    {
        if (x >= r + bit)
        {
            x -= r + bit;
            r = (r >> 1) + bit;
        }
        else
        {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, return 0 */

    seq_t count = 0;
    idx_t limit = isqrt_u64(n);     /* floor(sqrt(n)) */

    for (idx_t i = 1; i <= limit; ++i)
    {
        if (n % i == 0)
        {
            idx_t j = n / i;
            idx_t diff = (j > i) ? (j - i) : (i - j);
            idx_t s = isqrt_u64(diff);
            if (s * s == diff)
                count += (seq_t)((i == j) ? 1 : 2);
        }
    }
    return count;
}
