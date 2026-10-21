/*
 * A277425  a(n) = sqrt(16*t^2 - 32*t + k^2 + 8*k - 8*k*t + 16), where t =
 *          ceiling(sqrt(n)) and k = t^2 - n.
 * https://oeis.org/A277425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - (floor(sqrt(n-1))-1)^2
 * Range:     u8 n <= 4224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square-root helper: floor(sqrt(x)) for uint64_t input */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    /* Uses bitwise method, valid for 0 <= x < 2^64 */
    idx_t r = 0;
    idx_t bit = ((idx_t)1 << 62);          /* highest power of four <= 2^64 */
    while (x < bit) bit >>= 2;
    while (bit)
    {
        if (x >= r + bit)
        {
            x -= r + bit;
            r = (r >> 1) + bit;
        }
        else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1)                 /* a(1) = 0 explicitly */
        return (seq_t)0;

    /* s = floor(sqrt(n-1)) */
    idx_t s = floor_isqrt_u64(n - 1);
    /* val = s - 1 (non‑negative for n >= 2) */
    idx_t val = s - 1;
    /* result = n - val^2 */
    seq_t res = (seq_t)n - (seq_t)val * (seq_t)val;
    return res;
}
