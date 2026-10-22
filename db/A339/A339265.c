/*
 * A339265  Expansion of Product_{n >= 1} (1 - x^(2*n))*(1 - x^(2*n-1))*(1 -
 *          x^(2*n+1)).
 * https://oeis.org/A339265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^{ floor(sqrt(n)) }
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t _isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;               /* highest power of 2 <= 2^64-1 */
    /* Bring bit down to the largest power of 2 <= x */
    while (bit > x)
        bit >>= 2;
    /* Binary method for integer square root */
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
    idx_t k = _isqrt_u64(n);
    return (k & 1) ? (seq_t)-1 : (seq_t)1;
}
