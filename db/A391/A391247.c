/*
 * A391247  Numbers k such that there are no divisor pair (d, k/d) with 1 < d
 *          <= k/d for which k = A048720(k/d, A065621(d)).
 * https://oeis.org/A391247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that no d with 1 < d, d*d <= k, d | k has k =
 *            A048720(k/d, A065621(d)) (carryless GF(2) product of k/d and
 *            A065621(d)); found by a direct scan over k
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9061 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128
seq_rev_alt(uint64_t d)
{
    uint64_t low = d & (~d + 1);
    return (unsigned __int128)d ^ ((unsigned __int128)(d - low) << 1);
}

static int
seq_has_pair(uint64_t k)
{
    uint64_t d;
    for (d = 2; d <= k / d; d++) {
        if (k % d == 0) {
            unsigned __int128 r = seq_rev_alt(d);
            uint64_t x = k / d;
            unsigned __int128 res = 0;
            while (x) {
                if (x & 1)
                    res ^= r;
                r <<= 1;
                x >>= 1;
            }
            if (res == (unsigned __int128)k)
                return 1;
        }
    }
    return 0;
}

seq_t a(idx_t n)
{
    uint64_t k = 0;
    idx_t cnt = 0;
    while (cnt < n) {
        k++;
        if (!seq_has_pair(k))
            cnt++;
    }
    return (seq_t)k;
}
