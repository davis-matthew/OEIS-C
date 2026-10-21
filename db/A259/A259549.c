/*
 * A259549  Triangle T(n,k) with rows of length 2n-1 filled with consecutive
 *          integers, each appearing twice except for the last term, T(n,2n-1)
 *          = n(n+1)/2.
 * https://oeis.org/A259549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = ceil((n + floor(sqrt(n)))/2)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor sqrt for n < 2^64 */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low  = 0;
    idx_t high = (n >> 1) + 1;          /* sqrt(n) <= n/2 + 1 for n >= 2 */

    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;

        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                      /* offset is 1 */

    idx_t s = floor_sqrt_u64(n);                      /* floor(sqrt(n)) */
    idx_t t = n + s;                                   /* t = n + floor(sqrt(n)) */

    /* ceil(t/2) = (t+1)/2 for any integer t */
    seq_t res = (seq_t)((t + 1) >> 1);

    return res;
}
