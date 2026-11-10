/*
 * A349929  Numbers k such that A349509(k) = 3.
 * https://oeis.org/A349929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A349509(k) = 3; found by search over
 *            multiples of 3 (A349509(k)=3 iff k>1 and no carry occurs in base
 *            3 when adding k^3 and 6k^2-6k+3)
 * Range:     u8 n <= 14, u16 n <= 175, u32 n <= 2137, u64 n <= 2137,
 *            u128 n <= 2137, big any n
 * Validated: standard, pass, 67 terms through n = 1988 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A349509_is3(uint64_t k)
{
    if (k == 1)
        return false;
    unsigned __int128 k128 = (unsigned __int128)k;
    unsigned __int128 x = k128 * k128 * k128;
    unsigned __int128 y = (unsigned __int128)6 * k128 * k128
                         - (unsigned __int128)6 * k128
                         + (unsigned __int128)3;
    while (x || y)
    {
        unsigned d1 = (unsigned)(x % 3);
        unsigned d2 = (unsigned)(y % 3);
        if (d1 + d2 >= 3)
            return false;
        x /= 3;
        y /= 3;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    for (;;)
    {
        k += 3;
        if (A349509_is3(k))
        {
            count++;
            if (count == (uint64_t)n)
                return (seq_t)k;
        }
    }
}
