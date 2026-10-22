/*
 * A338024  Minimal number of moves for the cyclic variant of Hanoi's tower
 *          for 4 pegs and n disks, with the final peg one step away.
 * https://oeis.org/A338024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=1, a(2)=6, a(3)=15, a(4)=28, a(5)=57, a(6)=98, a(7)=179,
 *            a(8)=304, a(9)=521, a(10)=894, a(11)=1519. For n>11:
 *            a(n)=a(n-1)+a(n-2)+a(n-3)-2*a(n-5)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t cache[10001] = {0};
static idx_t cache_max = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* out of offset, not used */

    if (n <= cache_max) return cache[n];

    idx_t start = cache_max + 1;
    if (start < 1) start = 1;

    static const seq_t init[12] = {
        0ULL,        /* dummy 0 index */
        1ULL, 6ULL, 15ULL, 28ULL, 57ULL,
        98ULL, 179ULL, 304ULL, 521ULL,
        894ULL, 1519ULL
    };

    for (idx_t i = start; i <= n; ++i) {
        if (i <= 11) {
            cache[i] = init[i];
        } else {
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                       - (seq_t)2 * cache[i-5];
        }
    }

    cache_max = n;
    return cache[n];
}
