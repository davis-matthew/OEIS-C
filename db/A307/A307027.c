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
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* ---------- count undirected paths for given m <= n in K_{m,n} ---------- */
static inline seq_t count_paths_internal(seq_t m, seq_t n)
{
    seq_t total = 0;

    if (m == 0 || n == 0) return 0;

    /* --- start on the m side ------------------------------------------------ */
    seq_t prod = m * n;            /* length 1 (one edge) */
    total += prod;
    idx_t usedM = 1, usedN = 1;    /* vertices already used */

    for (idx_t e = 2; ; ++e) {     /* ex = number of edges (e) */
        if (e & 1) {               /* odd edge → vertex from n side */
            if (usedN >= n) break;
            prod *= (n - (seq_t)usedN);
            ++usedN;
        } else {                   /* even edge → vertex from m side */
            if (usedM >= m) break;
            prod *= (m - (seq_t)usedM);
            ++usedM;
        }
        total += prod;
    }

    /* --- start on the n side ---------------------------------------------- */
    prod = m * n;
    total += prod;
    usedM = 1;
    usedN = 1;

    for (idx_t e = 2; ; ++e) {
        if (e & 1) {               /* odd edge → vertex from m side */
            if (usedM >= m) break;
            prod *= (m - (seq_t)usedM);
            ++usedM;
        } else {                   /* even edge → vertex from n side */
            if (usedN >= n) break;
            prod *= (n - (seq_t)usedN);
            ++usedN;
        }
        total += prod;
    }

    return total >> 1;             /* each undirected path counted twice */
}

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
