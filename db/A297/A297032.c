/*
 * A297032  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-4
 *          digits of n; see Comments.
 * https://oeis.org/A297032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of monotonic pieces in the base‑4 digits of n
 * (treating blocks of equal consecutive digits as a separate piece). */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Extract base‑4 digits, least‑significant first */
    unsigned int d[64];
    size_t len = 0;
    idx_t x = n;
    while (x > 0) {
        d[len++] = (unsigned int)(x & 3);   /* x % 4 */
        x >>= 2;                            /* x /= 4 */
    }
    if (len <= 1)
        return (seq_t)0;

    size_t pieces = 1;
    int trend = 0;   /*  1 = ascent,  -1 = descent,   0 = equal */

    for (size_t i = len - 1; i > 0; --i) {
        int cur = d[i];
        int nxt = d[i - 1];
        int dir = (cur < nxt) ? 1 : (cur > nxt) ? -1 : 0;

        if (i == len - 1) {        /* first pair sets the trend */
            trend = dir;
            continue;
        }
        if (dir != trend) {
            ++pieces;
            trend = dir;
        }
    }
    return (seq_t)pieces;
}
