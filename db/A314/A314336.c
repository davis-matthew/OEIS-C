/*
 * A314336  Coordination sequence Gal.4.121.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A314336 – Coordination sequence Gal.4.121.4
    This function computes a(n) using the following recurrence:
        a(0) = 1
        for n >= 1 let k = n / 5 and r = n % 5
        d(n) = differences between successive terms:
            if (r == 0)  d(n) = 5 + 2*k
            if (r == 1)  d(n) = (n == 1) ? 4 : 5 - 2*k
            if (r == 2)  d(n) = 2
            if (r == 3)  d(n) = 7 + 2*k
            if (r == 4)  d(n) = 1 - 2*k
        a(n) = a(n-1) + d(n)
    The loop runs in O(n) time and works for all n >= 0, with the
    sequence values fitting easily into the seq_t type used by the
    build system.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    int64_t val = 1;          /* current value of a(i) */
    for (idx_t i = 1; i <= n; ++i)
    {
        int64_t diff;
        idx_t k = i / 5;
        int r = (int)(i % 5);

        switch (r)
        {
        case 0:
            diff = (int64_t)5 + 2 * (int64_t)k;
            break;
        case 1:
            diff = (i == 1) ? 4 : (int64_t)5 - 2 * (int64_t)k;
            break;
        case 2:
            diff = 2;
            break;
        case 3:
            diff = (int64_t)7 + 2 * (int64_t)k;
            break;
        case 4:
            diff = 1 - 2 * (int64_t)k;
            break;
        default:
            diff = 0;   /* unreachable */
            break;
        }

        val += diff;
    }

    return (seq_t)val;
}
