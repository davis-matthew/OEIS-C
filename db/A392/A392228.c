/*
 * A392228  Array read by antidiagonals: Place k points in general position on
 *          each side of a regular n-gon and join every pair of the k*n
 *          boundary points by a chord; T(n,k) (n >= 3, k >= 0) gives the
 *          number of regions in the resulting planar graph.
 * https://oeis.org/A392228
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A392228: Array read by antidiagonals.
   Let idx = n‑3, and write idx = s*(s+1)/2 + t  with 0 ≤ t ≤ s.
   Then let i = t, k = s‑t and n₀ = i+3.
   The value is T(n₀,k) where
       T(n,k) = 1 + k²·n·(n‑1)·(k²·(n²+n‑3) – 6k·(n‑1) + 15)/24.
   See the OEIS entry for the source of this formula. */

#include <stdint.h>

static idx_t int_isqrt_u64(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        r++;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                 /* offset is 3; values below are unused */

    idx_t idx = n - 3;                /* linear index in the antidiagonal order */
    idx_t tmp  = 1 + 8 * idx;         /* value for computing sqrt(1+8*idx) */
    idx_t s    = (int_isqrt_u64(tmp) - 1) / 2;  /* upper bound for s */

    /* adjust s to satisfy s*(s+1)/2 ≤ idx < (s+1)*(s+2)/2 */
    while ((s + 1) * (s + 2) / 2 <= idx)
        s++;
    while (s * (s + 1) / 2 > idx)
        s--;

    idx_t offset = idx - s * (s + 1) / 2;    /* t in the diagonal */
    idx_t i = offset;                        /* i runs from 0 to s   */
    idx_t k = s - offset;                     /* k = s – i            */
    idx_t n0  = i + 3;                        /* actual n of the term */

    seq_t K = (seq_t)k;
    seq_t N = (seq_t)n0;

    seq_t inside = K * K * (N * N + N - 3)
                 - 6 * K * (N - 1)
                 + 15;
    seq_t term2 = K * K * N * (N - 1) * inside / 24;
    seq_t result = 1 + term2;
    return result;
}
