/*
 * A260394  Infinite palindromic word (a(1),a(2),a(3),...) with initial word
 *          w(1) = (0,1,1) and midword sequence (a(n)); see Comments.
 * https://oeis.org/A260394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - A260444(n) Sequence definition: a(n) is the infinite
 *            word obtained by w(1)=011 and w(t+1)=w(t)+m(t)+w(t)*, where
 *            m(t)=a(t) (palindromic). The following function computes a(n)
 *            for any n≥1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   highest_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/highest_power_of_two.h"

static const seq_t base[3] = {0, 1, 1};

seq_t a(idx_t n) {
    if (n <= 3) {
        return base[n - 1];
    }
    /* check if n is a power of two */
    if ((n & (n - 1)) == 0) {
        /* n = 2^e, return a(e-1) */
        uint64_t e = 63 - __builtin_clzll((unsigned long long)n);
        return a((idx_t)(e - 1));
    }
    /* n is not a power of two */
    idx_t p = highest_power_of_two(n);        /* p <= n < 2p */
    /* compute n' = 2*p - n without overflow */
    unsigned __int128 two_p = ((unsigned __int128)1) << ((__builtin_clzll((unsigned long long)n) ^ 0x3F) + 1);
    unsigned __int128 np128 = two_p - (unsigned __int128)n;
    idx_t np = (idx_t)np128;
    return a(np);
}
