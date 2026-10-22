/*
 * A360552  Numbers > 1 whose distinct prime factors have integer median.
 * https://oeis.org/A360552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A360552 – numbers >1 whose distinct prime factors have integer median.
   Computation: iterate integers >1, skip those of the forbidden form 2^a * p^b
   (a ≥ 1, b ≥ 1, p odd prime).  All other integers are included. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static bool is_excluded(uint64_t x)
{
    /* must be even to be excluded */
    if ((x & 1) != 0)
        return false;

    /* strip all factors of 2 */
    uint64_t r = x;
    while ((r & 1) == 0)
        r >>= 1;

    /* pure powers of 2 are not excluded */
    if (r == 1)
        return false;

    /* check whether r is a power of a single odd prime */
    uint64_t t = r;
    size_t cnt = 0;
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            cnt++;
            while (t % p == 0)
                t /= p;
            if (cnt > 1)
                break;
        }
    }
    if (t > 1)
        cnt++;

    return cnt == 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t found = 0;
    for (idx_t m = 2; ; ++m) {
        if (is_excluded((uint64_t)m))
            continue;

        ++found;
        if (found == n)
            return (seq_t)m;
    }
}
