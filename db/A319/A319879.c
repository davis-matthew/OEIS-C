/*
 * A319879  a(n) = minimal number m of unit squares needed to make a figure
 *          formed from squares (joined edge to edge) which has n holes.
 * https://oeis.org/A319879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n >= 1, a(n) = 3*(n+1) + 2*floor(sqrt(4*n-3))
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for __int128 */


static idx_t isqrt_u128(__int128 v)
{
    /* binary search for floor(sqrt(v)) where v fits in 128 bits.
       The result always fits in 64 bits (its square <= v < 2^128). */
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << 32);   /* 2^32 > sqrt(2^128-1) */
    idx_t ans  = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;

        if (sq <= v) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* safety for very small v */
            high = mid - 1;
        }
    }
    return ans;
}


seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;      /* a(0) = 1 */

    /* 4*n - 3 with 128‑bit intermediate to avoid overflow. */
    __int128 val = (__int128)4 * n - 3;

    idx_t k = isqrt_u128(val);          /* floor(sqrt(4*n-3)) */

    seq_t nn   = (seq_t)n;
    seq_t res  = 3 * (nn + (seq_t)1) + 2 * (seq_t)k;

    return res;
}
