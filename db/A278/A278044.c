/*
 * A278044  Length of tribonacci representation of n (cf. A278038).
 * https://oeis.org/A278044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = index_of_largest_tribonacci_leq_n - 2, with a(0) = 1
 * Range:     u8 n <= 8509, u16 n <= 8509, u32 n <= 8509, u64 n <= 8509,
 *            u128 n <= 8509, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* static array to cache Tribonacci numbers up to the maximal needed */
    static idx_t trib[200];
    static size_t last = 0;   /* number of initialized terms */

    if (last == 0) {
        trib[0] = 0;
        trib[1] = 0;
        trib[2] = 1;
        last = 3;
    }

    /* extend tribonacci sequence until the next term would exceed n */
    while (1) {
        if (last + 1 >= sizeof(trib) / sizeof(trib[0])) break;
        idx_t next = trib[last - 1] + trib[last - 2] + trib[last - 3];
        if (next > n) break;
        trib[last] = next;
        ++last;
    }

    /* p is the index of the largest tribonacci number <= n */
    idx_t p = last - 1;

    /* If for some reason the last stored value is > n (overflow),
       search backwards to find the correct index. */
    if (trib[p] > n) {
        size_t lo = 0, hi = last - 1;
        while (lo < hi) {
            size_t mid = (lo + hi + 1) / 2;
            if (trib[mid] <= n)
                lo = mid;
            else
                hi = mid - 1;
        }
        p = lo;
    }

    return (seq_t)(p - 2);
}
