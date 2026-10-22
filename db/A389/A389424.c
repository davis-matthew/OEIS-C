/*
 * A389424  Array read by antidiagonals: T(n,k) is the carryless sum of n + n
 *          in base k, n >=0, k >= 2.
 * https://oeis.org/A389424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,k) where T(n,k) is the carry‑less sum of n+n in
 *            base k. The indices row and k are obtained by mapping the linear
 *            index n to the antidiagonal table: n = t(t+1)/2 + row, k =
 *            t-row+2
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   carryless_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements A389424 exactly as defined. */

#include <stdint.h>
#include "../utils/carryless_sum.h"

/* Integer square root of an unsigned 128‑bit value via binary search. */
static idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0, hi = 1;
    /* Expand hi until hi*hi > x. */
    while ((unsigned __int128)hi * hi <= x) hi <<= 1;
    /* Binary search between lo and hi. */
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= x) lo = mid;
        else hi = mid;
    }
    return lo;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find t such that t(t+1)/2 <= n < (t+1)(t+2)/2. */
    unsigned __int128 tmp = (unsigned __int128)n * 8 + 1;
    idx_t sqrt_val = isqrt_u128(tmp);          /* floor(sqrt(8*n+1)) */
    idx_t t = (sqrt_val - 1) / 2;              /* diagonal number */

    /* Number of elements before this diagonal. */
    idx_t prev_count = (unsigned __int128)t * (t + 1) / 2;
    idx_t offset = n - prev_count;             /* position within diagonal */

    idx_t row = offset;                        /* n index in T(n,k) */
    idx_t k   = t - row + 2;                    /* base (k >= 2) */

    return carryless_sum(row, k);
}
