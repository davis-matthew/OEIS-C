/*
 * A385000  Square array read by upward antidiagonals: A(n,k) = 0 except for
 *          A(d*(m-1),m*(d-1)) = d, with n >= 0, k >= 0, d >= 1, m >= 1.
 * https://oeis.org/A385000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   read i‑th element from the square array A where A(d*(m-1),
 *            m*(d-1)) = d (otherwise 0). The array is read by upward
 *            antidiagonals: (i,j) with i+j = s, i decreasing
 * Range:     u8 n <= 9616, u16 n <= 9616, u32 n <= 9616, u64 n <= 9616,
 *            u128 n <= 9616, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(x)) for 64‑bit values */
static inline idx_t isqrt_idx(idx_t x) {
    idx_t lo = 0, hi = ((idx_t)1 << 32);           /* hi² > 2⁶⁴‑1 */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;                                 /* lo is first > sqrt(x) */
}

/* compute value at array position (row, col) */
static inline seq_t array_value(idx_t row, idx_t col) {
    if (row == 0)                /* all values on first row are d = col+1 */
        return (seq_t)(col + 1);

    /* try every divisor d of 'row' */
    idx_t limit = isqrt_idx(row);
    for (idx_t d = 1; d <= limit; ++d) {
        if (row % d == 0) {
            /* divisor d */
            idx_t m  = row / d + 1;
            if (m * (d - 1) == col)
                return (seq_t)d;
            /* complementary divisor d2 */
            idx_t d2 = row / d;
            if (d2 != d) {
                idx_t m2 = row / d2 + 1;
                if (m2 * (d2 - 1) == col)
                    return (seq_t)d2;
            }
        }
    }
    return 0;  /* no matching d found */
}

/* main sequence function */
seq_t a(idx_t n) {
    /* determine diagonal s such that s(s+1)/2 <= n < (s+1)(s+2)/2 */
    __int128 temp = (__int128)8 * n + 1;
    idx_t t = isqrt_idx((idx_t)temp);            /* sqrt(8n+1) */
    idx_t s = (t - 1) >> 1;                      /* floor((t-1)/2) */

    idx_t first = (__int128)s * (s + 1) / 2;      /* first index of diag s */
    idx_t offset = n - first;                    /* position within diagonal */

    idx_t row = s - offset;                       /* i = s - offset */
    idx_t col = offset;                           /* j = offset */

    return array_value(row, col);
}
