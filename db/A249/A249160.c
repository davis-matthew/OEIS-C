/*
 * A249160  Smallest number of iterations k such that
 *          A068527^(k)(n)=A068527^(k+1)(n).
 * https://oeis.org/A249160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A249160: the smallest k such that R^k(n)=R^(k+1)(n),
   where R(m)=ceil(sqrt(m))^2 - m.  The function follows the
   definition directly using an integer square‑root routine. */

#include <stdint.h>

/* Integer floor square root for a 64‑bit unsigned integer.
   Uses binary search and __int128 to avoid overflow. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << ((sizeof(idx_t) * 8 + 1) / 2);  /* > sqrt(max) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t cur = n;
    seq_t cnt = 0;

    while (cur != 0 && cur != 2) {
        idx_t s = floor_sqrt_u64(cur);
        /* Check if cur is a perfect square */
        if ((unsigned __int128)s * s == cur) {
            cur = 0;
        } else {
            idx_t nsq = (s + 1);
            nsq = nsq * nsq;  /* (s+1)^2, may overflow 64‑bit, but result fits in uint128 */
            cur = nsq - cur; /* distance to next perfect square */
        }
        cnt++;
    }
    return cnt;
}
