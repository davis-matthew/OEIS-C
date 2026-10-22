/*
 * A345356  Numbers k coprime to 30 such that ceiling(sqrt(k))^2 - k is a
 *          square.
 * https://oeis.org/A345356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer k that is coprime to 30 and
 *            satisfies (ceil(sqrt(k)))^2 - k is a perfect square
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;          /* hi > sqrt(2^64-1) */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;                              /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1;; ++k) {
        /* skip numbers divisible by 2, 3, or 5 */
        if ((k & 1ULL) == 0) continue;
        if ((k % 3ULL) == 0) continue;
        if ((k % 5ULL) == 0) continue;

        /* ceil(sqrt(k)) */
        idx_t sf = isqrt_u64(k);                /* floor(sqrt(k))   */
        idx_t m  = sf;
        if ((unsigned __int128)m * m < k) m++;   /* ceiling if needed */

        /* diff = m^2 - k */
        idx_t diff = (unsigned __int128)m * m - k;

        /* check if diff is a square */
        idx_t t = isqrt_u64(diff);
        if ((unsigned __int128)t * t == diff) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
