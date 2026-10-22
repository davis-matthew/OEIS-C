/*
 * A329848  Beatty sequence for (13+sqrt(89))/8.
 * https://oeis.org/A329848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(13+sqrt(89))/8)
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>

/* integer square root for unsigned __int128 values.
   Returns floor(sqrt(x)). */
static inline unsigned long long floor_sqrt_u128(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = ULLONG_MAX, res = 0;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

/* Beatty sequence a(n) = floor(n*(13+sqrt(89))/8) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1, for safety */
    /* floor(n*sqrt(89)) = floor(sqrt(n^2*89)) */
    __int128 prod = (__int128)n * n * 89;
    unsigned long long sqrt_val = floor_sqrt_u128((unsigned __int128)prod);
    seq_t term = ((seq_t)13 * n + (seq_t)sqrt_val) / 8;
    return term;
}
