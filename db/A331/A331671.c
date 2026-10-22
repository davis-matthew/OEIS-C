/*
 * A331671  Number of Pythagorean triangles with sum of legs n.
 * https://oeis.org/A331671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of Pythagorean triples with sum of legs n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for unsigned long long, using a binary‑search method. */
static inline unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long r = 0, bit = 1ULL << 62;   /* highest power of 4 <= 2^64 */
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
    seq_t cnt = 0;
    idx_t limit = (n - 1) / 2;
    for (idx_t k = 1; k <= limit; ++k)
    {
        idx_t a_leg = n - k;                    /* larger leg */
        unsigned long long sum = (unsigned long long)a_leg * a_leg
                               + (unsigned long long)k * k;
        unsigned long long t = isqrt_u64(sum);
        if (t * t == sum)
            cnt = cnt + 1;
    }
    return cnt;
}
