/*
 * A287166  Smallest number with exactly n representations as a sum of 7
 *          nonzero squares or 0 if no such number exists.
 * https://oeis.org/A287166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* r(s) = the number of ways to write s as an unordered sum of 7 positive
 * squares, for all s <= L, by the usual DP over the squares in increasing
 * order (dp[k][s] += dp[k-1][s - q], s ascending, so a square may repeat).
 * a(n) is the least s with r(s) = n, or 0 when there is none. r grows like
 * s^(5/2), so once every r(s) on [L/2, L] exceeds n no larger s can have n
 * representations; L doubles until then. The counts fit in 64 bits for the
 * L needed. (The original kept the DP in seq_t, too slow under big, and when
 * it extended the table added only the new squares to the new sums.) */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *R = NULL;                   /* R[s] = r(s), s <= L */
static uint64_t L = 0;

static void build(uint64_t lim)
{
    uint64_t *dp = (uint64_t *)calloc(8 * (lim + 1), sizeof *dp);
    if (!dp) abort();
    dp[0] = 1;                               /* dp[k * (lim + 1) + s] */
    for (uint64_t i = 1; i * i <= lim; ++i) {
        uint64_t q = i * i;
        for (uint64_t s = q; s <= lim; ++s)
            for (int k = 7; k >= 1; --k)
                dp[k * (lim + 1) + s] += dp[(k - 1) * (lim + 1) + s - q];
    }
    free(R);
    R = (uint64_t *)malloc((lim + 1) * sizeof *R);
    if (!R) abort();
    for (uint64_t s = 0; s <= lim; ++s) R[s] = dp[7 * (lim + 1) + s];
    free(dp);
    L = lim;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    for (;;) {
        if (!L) build(1024);
        uint64_t lo = UINT64_MAX;
        for (uint64_t s = L / 2; s <= L; ++s)
            if (R[s] < lo) lo = R[s];
        if (lo > (uint64_t)n) break;
        build(2 * L);
    }
    for (uint64_t s = 1; s <= L; ++s)
        if (R[s] == (uint64_t)n) return (seq_t)s;
    return (seq_t)0;
}
