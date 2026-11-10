/*
 * A344769  a(n) = A005187(n) - A011772(n).
 * https://oeis.org/A344769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005187(n) - A011772(n), with A005187(n) = 2n -
 *            popcount(n) and A011772(n) = least m>=1 with n | m(m+1)/2 (found
 *            by search over m < 2n)
 * Range:     u8 n <= 144, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 98 terms through n = 9317 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A344769_smallest_tri(idx_t n)
{
    idx_t limit = 2 * n;
    for (idx_t m = 1; m < limit; ++m) {
        idx_t x = m, y = m + 1;
        if ((x & 1) == 0) x >>= 1; else y >>= 1;
        unsigned __int128 prod = (unsigned __int128)x * y;
        if ((prod % n) == 0)
            return (seq_t)m;
    }
    return (seq_t)(limit - 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t pop = 0;
    idx_t t = n;
    while (t) { pop += (seq_t)(t & 1); t >>= 1; }
    seq_t m = A344769_smallest_tri(n);
    return (seq_t)n * 2 - (pop + m);
}
