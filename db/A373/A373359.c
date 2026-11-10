/*
 * A373359  Irregular triangle read by rows where row n lists (in increasing
 *          order) the elements of the Schreier set encoded by A371176(n).
 * https://oeis.org/A373359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, base
 * Formula:   T(n,k) = A373579(n,k) - 1; row n lists, in increasing order, the
 *            1-based bit positions of the set bits of A371176(n), where
 *            A371176 is the numbers m with popcount(m) <= ctz(m)+1. Computed
 *            by a linear scan over m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9643 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t k = 0;
    idx_t cum = 0;
    if (n < 1) return (seq_t)0;
    while (true)
    {
        k++;
        unsigned pop = (unsigned)__builtin_popcountll(k);
        unsigned tz = (unsigned)__builtin_ctzll(k);
        if (pop <= tz + 1)
        {
            if (cum + pop >= n)
            {
                unsigned want = (unsigned)(n - cum); /* 1-based within row */
                unsigned idx = 0;
                for (unsigned bit = 0; bit < 64; bit++)
                {
                    if ((k >> bit) & 1)
                    {
                        idx++;
                        if (idx == want) return (seq_t)(bit + 1);
                    }
                }
            }
            cum += pop;
        }
    }
}
