/*
 * A293052  Rectangular array by antidiagonals: T(n,m) = rank of n*sqrt(3)+m
 *          when all the numbers k*sqrt(3)+h, for k >= 1, h >= 0, are jointly
 *          ranked.
 * https://oeis.org/A293052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   * a(n) = T(row, col) where (row, col) are obtained by traversing
 *            a * rectangular array antidiagonally: * * S = smallest integer
 *            such that S*(S+1)/2 ≥ n * pos = n - S*(S-1)/2 - 1 // 0‑based
 *            position inside the S‑th diagonal * row = pos + 1 * col = S-1 -
 *            pos * * T(n,m) = (n + ⌊m/√3⌋)·(m+1) + Σ_{t=1...
 * Range:     u8 n <= 7, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Validated: standard, pass, 83 terms through n = 1701 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static inline idx_t floor_m_over_sqrt3(idx_t m)
{
    /* ⌊m/√3⌋ = ⌊m·√3⌋ / 3,  ⌊m·√3⌋ = floor_sqrt(3·m²) */
    seq_t t = floor_sqrt((seq_t)3 * m * m);
    return (idx_t)(t / 3);
}

static inline seq_t floor_t_sqrt3(idx_t t)
{
    /* ⌊t√3⌋ = floor_sqrt(3·t²) */
    return floor_sqrt((seq_t)3 * t * t);
}

static seq_t T_value(idx_t n, idx_t m)
{
    idx_t L = floor_m_over_sqrt3(m);
    seq_t part1 = (seq_t)(n + L) * ((seq_t)m + 1);

    seq_t sum_nminus1 = 0;
    for (idx_t t = 1; t < n; ++t)
        sum_nminus1 += floor_t_sqrt3(t);

    seq_t sum_L = 0;
    for (idx_t t = 1; t <= L; ++t)
        sum_L += floor_t_sqrt3(t);

    return part1 + sum_nminus1 - sum_L - (seq_t)L;
}

seq_t a(idx_t n)
{
    /* ---- determine (row, col) from antidiagonal enumeration ---- */
    seq_t N = (seq_t)n;
    seq_t eightNplus1 = (seq_t)8 * N + 1;
    seq_t w = floor_sqrt(eightNplus1);

    idx_t S; /* length of the diagonal (number of elements) */
    if (w * w == eightNplus1)
        S = (idx_t)((w - 1) / 2);
    else
        S = (idx_t)((w + 1) / 2);

    idx_t prev = (S - 1) * S / 2;          /* elements before this diagonal */
    idx_t pos = n - prev - 1;              /* 0‑based position in the diagonal */

    idx_t row = pos + 1;
    idx_t col = (S - 1) - pos;

    /* ---- compute T(row, col) ---- */
    return T_value(row, col);
}
