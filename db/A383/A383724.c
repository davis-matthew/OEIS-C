/*
 * A383724  a(n) = A378762(A382680(n)).
 * https://oeis.org/A383724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A378762(A382680(n)), computed via the triangle array
 *            definition: Row r has length L = 4r-3. The cumulative count up
 *            to row r is C(r) = 2r² - r. Let r be the unique integer such
 *            that C(r-1) < n ≤ C(r). With k = n - C(r-1), let m = 2r - 1. The
 *            offset P is defined by P = m + k (k < m, odd) P...
 * Range:     u8 n <= 233, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern idx_t _floor_isqrt_u64(idx_t n);

seq_t a(idx_t n)
{
    /* Compute the row r such that 2*(r-1)^2-(r-1) < n <= 2*r^2-r   */
    idx_t low = 1;
    idx_t high = 1;
    /* Find an upper bound where condition holds */
    while (1) {
        __uint128_t val = (__uint128_t)2 * high * high - high;
        if (val >= n) break;
        high <<= 1;   /* double until condition satisfied */
    }
    /* Binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)2 * mid * mid - mid;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                 /* row index (1‑based) */
    idx_t t = r - 1;               /* t = r-1 for formulas below */
    /* Cumulative count up to previous row */
    __uint128_t c_prev128 = (__uint128_t)2 * t * t - t;
    idx_t C_prev = (idx_t)c_prev128;   /* fits in idx_t (n ≥ 1) */
    idx_t k = n - C_prev;         /* position within the row (1‑based) */
    idx_t m = 2 * r - 1;          /* odd integer */

    seq_t P;
    if (k < m) {
        if (k & 1)  /* odd */
            P = (seq_t)(m + k);
        else        /* even */
            P = (seq_t)k;
    } else {
        if (k & 1)  /* odd */
            P = (seq_t)(3 * m - 1 - k);
        else        /* even */
            P = (seq_t)(k - m);   /* k ≥ m, so non‑negative */
    }

    /* A000384(t) = t*(2t-1)  (hexagonal numbers) */
    __uint128_t base128 = (__uint128_t)t * ( (__uint128_t)2 * t - 1 );
    seq_t base = (seq_t)base128;

    return base + P;
}
