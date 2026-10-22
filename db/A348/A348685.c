/*
 * A348685  Triangle read by rows, T(n, k) = 2^(2*n - HammingWeight(k)), for 0
 *          <= k <= n.
 * https://oeis.org/A348685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(2*r - popcount(k)) where r is the row index satisfying
 *            * r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Binary search for the row r. The maximum possible r is n, so hi = n. */
    idx_t lo = 0;
    idx_t hi = n;                      /* hi is always >= the desired r */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;   /* upper mid to avoid infinite loop */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= (unsigned __int128)n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;

    /* Compute column index k. */
    unsigned __int128 pref = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)pref;

    /* Hamming weight of k. */
    unsigned int pc = __builtin_popcountll((unsigned long long)k);

    /* Exponent for the power of two. */
    idx_t exp = 2 * r - pc;

    return pow2(exp);
}
