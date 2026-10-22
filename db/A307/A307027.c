/*
 * A307027  Number of (undirected) paths in the complete bipartite graph
 *          K_{m,n} (triangle read by rows with m = 1..n and n = 1..).
 * https://oeis.org/A307027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   count undirected paths in K_{m,n} by summing directed paths and
 *            dividing by two. The triangle is read row‑by‑row with m = 1..n
 *            and n = 1.. . The index n of this function refers to the
 *            flattened sequence
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 35, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   count_paths_internal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/count_paths_internal.h"

/* ---------- helper: integer square root (floor) for 64‑bit values ---------- */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = (idx_t)1ULL << 32;          /* 2³² > sqrt(2⁶⁴) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* -------------------------- main sequence function ------------------------ */
seq_t a(idx_t n)
{
    /* Determine row (n_part) and column (m_part) in the triangle */
    idx_t k = isqrt_u64(1 + 8 * n);               /* floor sqrt(1+8n)          */
    idx_t row = (k - 1) / 2;                      /* initial guess for r       */
    while ((unsigned __int128)row * (row + 1) / 2 < n)
        ++row;                                    /* adjust to smallest r with T_r >= n */

    idx_t prev_tri = (row - 1) * row / 2;         /* T_{r-1}                   */
    idx_t m = n - prev_tri;                       /* column index (1..row)     */

    return count_paths_internal((seq_t)m, (seq_t)row);
}
