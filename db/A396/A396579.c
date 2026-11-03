/*
 * A396579  Nonnegative integers that contain a square as a suffix.
 * https://oeis.org/A396579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: seq_t a(idx_t n) returns the n-th non‑negative integer that ends
   with a decimal perfect square suffix.  We sequentially test each integer
   starting from 0 for a square suffix and count the matches until the n‑th
   match is found. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static bool has_square_suffix(idx_t m)
{
    /* precomputed powers of ten up to 10^18 to avoid recomputation */
    static const idx_t pow10[19] =
    {1ULL,10ULL,100ULL,1000ULL,10000ULL,100000ULL,1000000ULL,
     10000000ULL,100000000ULL,1000000000ULL,10000000000ULL,
     100000000000ULL,1000000000000ULL,10000000000000ULL,
     100000000000000ULL,1000000000000000ULL,10000000000000000ULL,
     100000000000000000ULL,1000000000000000000ULL};

    /* determine the number of decimal digits of m */
    idx_t tmp = m;
    int digits = 1;
    while (tmp >= 10)
    {
        tmp /= 10;
        ++digits;
    }

    for (int k = 1; k <= digits; ++k)
    {
        idx_t suffix = m % pow10[k];
        seq_t s = (seq_t)suffix;
        seq_t r = floor_sqrt(s);
        if (r * r == s)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;
    while (true)
    {
        if (has_square_suffix(m))
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }
}
