/*
 * A334751  a(n) is the number immediately above n in a clockwise square
 *          spiral of the positive integers with the first step to the right.
 * https://oeis.org/A334751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the integer immediately above n in the clockwise square
 *            spiral * that starts by stepping to the right. The
 *            implementation converts n to its * coordinate in the spiral,
 *            moves one step north, and converts the new coordinate * back to
 *            the spiral number
 * Range:     u8 n <= 210, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ring_of_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ring_of_n.h"

static void n_to_xy(idx_t n, int64_t *x, int64_t *y)
{
    idx_t k = ring_of_n(n);
    if (k == 0)
    {
        *x = 0;
        *y = 0;
        return;
    }
    unsigned __int128 side = ((unsigned __int128)k)*2 + 1;
    unsigned __int128 max_val = side * side;
    uint64_t t = (uint64_t)(max_val - (unsigned __int128)n);  /* 0 <= t < 8k */

    if (t < 2 * k)
    {
        *x = (int64_t)(k - t);
        *y = (int64_t)k;
    }
    else if (t < 4 * k)
    {
        t -= 2 * k;
        *x = -(int64_t)k;
        *y = (int64_t)(k - t);
    }
    else if (t < 6 * k)
    {
        t -= 4 * k;
        *x = (int64_t)(-k + t);
        *y = -(int64_t)k;
    }
    else
    {
        t -= 6 * k;
        *x = (int64_t)k;
        *y = (int64_t)(-k + t);
    }
}

static seq_t coord_to_num(int64_t x, int64_t y)
{
    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    idx_t k = ax > ay ? ax : ay;
    if (k == 0)
        return (seq_t)1;

    unsigned __int128 side = ((unsigned __int128)k)*2 + 1;
    unsigned __int128 max_val = side * side;
    uint64_t t;

    if (y == (int64_t)k)
        t = (uint64_t)(k - x);
    else if (x == -(int64_t)k)
        t = (uint64_t)(2*k + (k - y));
    else if (y == -(int64_t)k)
        t = (uint64_t)(4*k + (x + k));
    else   /* x == k */
        t = (uint64_t)(6*k + (y + k));

    unsigned __int128 val = max_val - t;
    return (seq_t)val;
}

seq_t a(idx_t n)
{
    int64_t x, y;
    n_to_xy(n, &x, &y);
    return coord_to_num(x, y + 1);
}
