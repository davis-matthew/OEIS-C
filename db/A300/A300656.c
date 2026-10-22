/*
 * A300656  Triangle read by rows: T(n,k) = 30*k^2*(n-k)^2 + 1; n >= 0, 0 <= k
 *          <= n.
 * https://oeis.org/A300656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 30*k^2*(n-k)^2 + 1, * where n = floor((sqrt(8*i+1)-1)/2)
 *            and k = i - n*(n+1)/2. * The sequence is obtained by reading the
 *            triangle T(n,k)=30*k^2*(n-k)^2+1 * by rows, so a(i) = T(n,k)
 *            with i the row‑major index
 * Range:     u8 n <= 10, u16 n <= 110, u32 n <= 2079, u64 n <= 2079,
 *            u128 n <= 2079, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root: returns the largest s such that s*s <= x. */
static inline idx_t floor_sqrt(idx_t x)
{
    /* Upper bound for sqrt(UINT64_MAX) is 2^32. */
    idx_t lo = 0, hi = ((idx_t)1 << 32) - 1;
    if (x < hi*hi) hi = x;          /* tighten high if x is small */
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t sq = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Determine triangular row n_tri and column k for linear index n. */
    idx_t i = n;
    idx_t discr = 8 * i + 1;                 /* 8*i+1  (fits 64‑bit for all required n) */
    idx_t s = floor_sqrt(discr);
    idx_t n_tri = (s - 1) / 2;                /* floor((sqrt(8i+1)-1)/2) */
    idx_t row_start = n_tri * (n_tri + 1) / 2;
    idx_t k = i - row_start;

    /* Compute T(n_tri, k) = 30*k^2*(n_tri-k)^2 + 1. */
    seq_t k_s = (seq_t)k;
    seq_t m_s = (seq_t)(n_tri - k);
    seq_t val = k_s * k_s;     /* k^2 */
    val *= m_s * m_s;          /* * (n-k)^2  */
    val *= 30;                 /* * 30       */
    val += 1;                  /* + 1        */
    return val;
}
