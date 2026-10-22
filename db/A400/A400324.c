/*
 * A400324  Irregular triangle read by rows, where row n gives the lengths of
 *          consecutive same species move blocks in the solution of the Frogs
 *          and Toads interchange puzzle with n frogs and n toads.
 * https://oeis.org/A400324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, new
 * Formula:   a(n) is the sequence obtained by reading the irregular triangle
 *            T(r,k) = min(k, 2r+2-k, r) (1 ≤ k ≤ 2r+1) row‑by‑row. For a
 *            given index n (n ≥ 1) let r be the unique integer with r(r+2) ≥
 *            n > (r-1)r + 2(r-1) (i.e. the cumulative number of terms up to
 *            row r). Let k = n - ((r-1)(r-1)+2(r-1)); then...
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* guard for incomplete offset – not expected to be called with n==0 */
    if (n == 0) return (seq_t)0;

    /* Find r such that r(r+2) >= n.  Use binary search on [1, n].  */
    idx_t low  = 1;
    idx_t high = 1;
    while ((__uint128_t)high * (high + 2) < n) {
        high <<= 1;
        if (high > n) high = n;   /* avoid overflow */
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((__uint128_t)mid * (mid + 2) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* cumulative terms up to row r-1: (r-1)^2 + 2(r-1) */
    idx_t prev = (r - 1) * (r - 1) + 2 * (r - 1);

    idx_t k = n - prev;                 /* 1‑based position in row r */
    idx_t term = k;
    idx_t alt  = 2 * r + 2 - k;
    if (alt < term) term = alt;
    if (r   < term) term = r;

    return (seq_t)term;
}
