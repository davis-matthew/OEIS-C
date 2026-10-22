/*
 * A349703  Irregular triangle read by rows where T(n,k) is the number of free
 *          trees attaining the maximum terminal Wiener index (A349702) for a
 *          tree of n vertices among which k are leaves.
 * https://oeis.org/A349703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   * a(n) is the value of the triangle T(r,k) read row‑wise. * For
 *            rows r < 3, T(r,r) = 1. * For r ≥ 3, T(r,2) = 1, T(r,3) =
 *            round((r-1)²/12), * T(r,k) = 1 if k is even, * T(r,k) =
 *            ceil((r-k)/2) if k is odd and k ≥ 5. * The index n is mapped to
 *            (r,k) by the cumulative counts * C(r) = 1 + 1 + 1 + 3 +...
 * Range:     u8 n <= 1488, u16 n <= 9593, u32 n <= 9593, u64 n <= 9593,
 *            u128 n <= 9593, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* cumulative number of terms up to and including row r */
static inline uint64_t cumulative(uint64_t r)
{
    if (r < 3)
        return r + 1;
    /* use 128‑bit arithmetic to avoid overflow */
    __uint128_t x = (__uint128_t)(r - 2) * (r - 1);
    x = x / 2;
    x += 3;
    if (x > UINT64_MAX)
        return UINT64_MAX;            /* saturate */
    return (uint64_t)x;
}

/* main sequence function */
seq_t a(idx_t n)
{
    uint64_t idx = (uint64_t)n;

    /* rows 0,1,2 have a single entry = 1 */
    if (idx < 3)
        return (seq_t)1;

    /* locate row r such that cumulative(r-1) ≤ idx < cumulative(r) */
    uint64_t low = 3, high = 3;

    /* grow high until it exceeds idx */
    while (cumulative(high) <= idx) {
        if (high > UINT64_MAX / 2) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (cumulative(mid) <= idx)
            low = mid + 1;
        else
            high = mid;
    }
    uint64_t r = low;                 /* the required row */

    /* compute position within the row */
    uint64_t cum_prev = (r < 3) ? r : cumulative(r - 1);
    uint64_t pos = idx - cum_prev;    /* 0‑based */
    uint64_t k  = 2 + pos;            /* column in the row */

    /* compute T(r,k) */
    if (k == 3) {
        seq_t m = (seq_t)(r - 1);
        return (m * m + 6) / 12;
    }
    if ((k & 1) == 0)      /* even k */
        return (seq_t)1;
    /* odd k ≥ 5 */
    return (seq_t)((r - k + 1) / 2);
}
