/*
 * A396776  Square array read by upwards antidiagonals: T(p,q) is the number
 *          cells required to completely surround a single cell of the regular
 *          tiling with Schläfli symbol {p,q}.
 * https://oeis.org/A396776
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = p*(q-2) where (p,q) are read from the square array * with
 *            entries T(p,q) = p*(q-2) for p, q ≥ 3 along upward
 *            antidiagonals. * The offset is 3: a(3) is the first term
 *            (T(3,3))
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit value, returning the floor. */
static idx_t floor_sqrt_uint128(__int128 v) {
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 34) - 1;          /* √(1+8·max_n) < 2^34 for 64‑bit n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 m2 = (__int128)mid * mid;
        if (m2 <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;                     /* value for indices below the offset is irrelevant */

    /* L is the one‑based position of the term among all elements starting at a(3). */
    idx_t L = n - 2;

    /* Find minimal m such that m*(m+1)/2 ≥ L. */
    __int128 delta = 1 + (__int128)8 * L;
    idx_t sq = floor_sqrt_uint128(delta);
    idx_t m = (sq - 1) / 2;
    while ((__int128)m * (m + 1) / 2 < L) ++m;

    /* Sum p+q for the diagonal that contains a(n). */
    idx_t S = m + 5;

    /* Number of elements before this diagonal. */
    idx_t cum_before = ((__int128)(m - 1) * m) / 2;

    /* Position (zero‑based) inside the current diagonal. */
    idx_t pos = (n - 3) - cum_before;

    /* Compute p and q.  In each diagonal, p decreases while q increases. */
    idx_t p = S - 3 - pos;
    idx_t q = S - p;

    return (seq_t)p * (seq_t)(q - 2);
}
