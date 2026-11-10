/*
 * A284933  Positions of 0 in A284792; complement of A284934.
 * https://oeis.org/A284933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284792, the Thue-Morse word
 *            with each non-overlapping 1001 replaced by 1. Computed by
 *            scanning the transformed word
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9776 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline int A284933_tm(idx_t i)
{
    uint64_t x = (uint64_t)i;
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (int)(x & 1);
}

seq_t a(idx_t n)
{
    idx_t out = 0;   /* length of transformed word produced so far */
    idx_t pos = 0;   /* index in the Thue-Morse word */
    idx_t zeros = 0;

    for (;;)
    {
        int b0 = A284933_tm(pos);
        ++out;
        if (b0 == 1 && A284933_tm(pos + 1) == 0 && A284933_tm(pos + 2) == 0 && A284933_tm(pos + 3) == 1)
        {
            pos += 4;   /* emits 1 */
        }
        else
        {
            if (b0 == 0)
            {
                ++zeros;
                if (zeros == n) return (seq_t)out;
            }
            ++pos;
        }
    }
}
