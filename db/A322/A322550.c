/*
 * A322550  Table read by ascending antidiagonals: T(n, k) is the minimum
 *          number of cubes necessary to fill a right square prism with base
 *          area n^2 and height k.
 * https://oeis.org/A322550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = X(r,c) where X(r,c)=T(r+1-c,c) and *
 *            T(m,k)=m^2*k/gcd(m,k)^3. * The index n is unfolded into the
 *            lower‑triangular matrix * in row‑major order: find the smallest
 *            r with r(r+1)/2 ≥ n, * then c = n - r(r-1)/2. Return
 *            simplification * (m/g)^2*(k/g) with m = r+1-c and g = gcd(m,k)
 * Range:     u8 n <= 68, u16 n <= 2872, u32 n <= 9805, u64 n <= 9805,
 *            u128 n <= 9805, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute the nth term of OEIS A322550 */
seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 >= n */
    idx_t low = 1;
    idx_t high = n;                     /* high bound is never exceeded */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= (__int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                       /* current row */
    __int128 t_prev_128 = (__int128)(r - 1) * r / 2;
    idx_t t_prev = (idx_t)t_prev_128;    /* sum of lengths of previous rows */
    idx_t c = n - t_prev;                /* column within the row */

    /* m = r + 1 - c */
    idx_t m_idx = r + 1 - c;
    seq_t m = (seq_t)m_idx;
    seq_t k = (seq_t)c;

    /* Simplify m^2 * k / gcd(m,k)^3 as (m/g)^2 * (k/g) */
    seq_t g = gcd(m, k);
    seq_t m1 = m / g;
    seq_t k1 = k / g;
    return m1 * m1 * k1;
}
