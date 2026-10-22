/*
 * A396877  a(n) is the first step at which the n-th lattice point in
 *          counterclockwise square-spiral order becomes an endpoint in a
 *          growth process of alternating perpendicular bars starting from one
 *          horizontal bar.
 * https://oeis.org/A396877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 4; for n>0, let k = ceil((sqrt(n+1)-1)/2), m = (n -
 *            4*k*(k-1) - 1) mod (4*k), then a(n) = 2*k - 1 + (m mod 2) + 2*[m
 *            >= 2*k && m even]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root of an unsigned 64‑bit value, floor */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (n >> 1) + 1;        /* n may be 0 -> hi = 1 */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;

    /* compute k */
    idx_t s1 = n + 1;
    idx_t t  = isqrt_u64(s1);                /* floor sqrt(n+1) */
    idx_t k  = (t + 1) >> 1;                 /* (t+1)/2 */

    if ((t & 1) && ( (unsigned __int128)t * t == s1 )) {
        k -= 1;                               /* adjust for odd perfect square */
    }

    /* m = (n - 4*k*(k-1) - 1) mod (4*k) */
    unsigned __int128 sub128 = (unsigned __int128)4 * k * (k - 1);
    idx_t sub = (idx_t)sub128;
    idx_t rest = n - sub - 1;
    idx_t modBase = 4 * k;
    idx_t m = rest % modBase;

    /* compute a(n) */
    seq_t res = (seq_t)(2 * k - 1);           /* 2*k - 1 */
    res += (seq_t)(m & 1);                   /* + (m mod 2) */

    if (m >= 2 * k && ((m & 1) == 0)) {
        res += 2;                             /* + 2 if conditions met */
    }

    return res;
}
