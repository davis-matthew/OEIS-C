/*
 * A393705  a(n) is the number of ways to write n as the sum of a square and
 *          the reverse of a square.
 * https://oeis.org/A393705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of pairs (x, y) with x a perfect square and y * a
 *            reverse of a perfect square, such that x + y = n. * y is allowed
 *            to be 0; for y > 0 it must not end in 0, * because reverse(y)
 *            could not then be a square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/rev.h"

/* Integer square root by binary search (floor). */
static inline idx_t integer_sqrt(idx_t x)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;      /* high > sqrt(max idx_t) for 64‑bit only */
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Test whether x is a perfect square. */
static inline bool is_square(idx_t x)
{
    idx_t r = integer_sqrt(x);
    return ((unsigned __int128)r * r) == x;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t sqrt_n = integer_sqrt(n);
    for (idx_t i = 0; i <= sqrt_n; ++i)
    {
        idx_t x = (idx_t)i * i;
        idx_t y = n - x;
        /* For y > 0 it must not end with a zero; y == 0 is allowed. */
        if (y != 0 && (y % 10) == 0)
            continue;
        idx_t rev_y = rev(y);
        if (is_square(rev_y))
            ++count;
    }
    return count;
}
