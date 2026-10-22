/*
 * A309714  The smallest possible nonnegative difference between the sum of
 *          the first n positive integers (A000217) and the sum of any number
 *          of the directly following and consecutive integers.
 * https://oeis.org/A309714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_k { k*(2n + k + 1)/2 } subtracted from n*(n+1)/2
 *            where k is the largest integer >=0 such that k*(2n + k + 1)/2 <=
 *            n*(n+1)/2
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for unsigned __int128, floor */
static inline uint64_t isqrt_uint128(unsigned __int128 n)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 64;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (uint64_t)lo;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* sequence is 1‑offset; n>=1 in practice */
        return 0;

    __int128 m = n;                     /* use 128‑bit for intermediate math */

    /* compute D = 8*n*(n+1) + 1; then sqrt(D) */
    unsigned __int128 D = 8 * (__int128)m * (m + 1) + 1;
    uint64_t sqrtD = isqrt_uint128(D);

    /* k = floor( (- (2*n+1) + sqrt(D)) / 2 ) */
    __int128 k;
    if (sqrtD > (uint64_t)(2 * (uint64_t)m + 1))
        k = ((__int128)sqrtD - (2 * m + 1)) / 2;
    else
        k = 0;        /* for n=1 this makes k=0 (empty block) */

    if (k < 0) k = 0;

    /* S = n*(n+1)/2 */
    __int128 S = m * (m + 1) / 2;

    /* S' = k*(2*n + k + 1)/2 */
    __int128 Sprime = k * (2 * m + k + 1) / 2;

    return (seq_t)(S - Sprime);
}
