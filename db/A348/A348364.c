/*
 * A348364  Number of vertices on the axis of symmetry of the symmetric
 *          representation of sigma(n).
 * https://oeis.org/A348364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + A347950(n) a(n) = 2 if n has a middle divisor,
 *            otherwise a(n) = 1 (A mid‑divisor d satisfies ⌈n/2⌉ ≤ d² ≤ n.)
 * Range:     u8 n <= 9717, u16 n <= 9717, u32 n <= 9717, u64 n <= 9717,
 *            u128 n <= 9717, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* integer square root: floor(sqrt(x)) for uint64_t */
static idx_t _int_sqrt(idx_t x)
{
    if (x == 0 || x == 1)
        return x;
    idx_t r = 0;
    /* Find the highest power of four <= x */
    idx_t bit = ((idx_t)1 << 62);
    while (bit > x)
        bit >>= 2;
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

/* Check if n has a middle divisor */
static bool _has_middle_divisor(idx_t n)
{
    /* Upper bound for divisor: floor(sqrt(n)) */
    idx_t d_high = _int_sqrt(n);

    /* Lower bound: ceil(sqrt(ceil(n/2))) */
    idx_t ceil_half = (n + 1) / 2;
    idx_t d_low = _int_sqrt(ceil_half);
    if (d_low * d_low < ceil_half)
        d_low++;

    if (d_low > d_high)
        return false;

    for (idx_t d = d_low; d <= d_high; ++d)
    {
        if (n % d == 0)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    return _has_middle_divisor(n) ? (seq_t)2 : (seq_t)1;
}
