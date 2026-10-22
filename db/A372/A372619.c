/*
 * A372619  Square array T(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where T(n,k) = 1/(phi(k)) * Sum_{j=1..n} phi(k*j).
 * https://oeis.org/A372619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where T(i,j) = (1/φ(j))·∑_{k=1..i}φ(k·j), and
 *            (i,j) is chosen so that the terms are read by antidiagonals
 *            downwards with OEIS offset 1. (See sequence A372619.)
 * Range:     u8 n <= 220, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"

/* Find the smallest s ≥ 2 such that s·(s‑1)/2 ≥ n.  */
static inline idx_t
find_s(idx_t n)
{
    idx_t lo = 1, hi = 2;
    while ((unsigned __int128)hi * (hi - 1) / 2 < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Compute a(n) for OEIS sequence A372619. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                       /* terms before offset are undefined */

    /* Locate the antidiagonal (s = i + j) and the position on it. */
    idx_t s     = find_s(n);                       /* diagonal index (sum i+j)   */
    idx_t prev  = (s - 2) * (s - 1) / 2;           /* elements before this diag  */
    idx_t offset = n - prev - 1;                   /* 0‑based offset on diag    */
    idx_t i = offset + 1;                          /* row index                 */
    idx_t j = s - i;                               /* column index              */

    seq_t denom = calc_phi((seq_t)j);               /* φ(j)                      */
    seq_t sum   = 0;
    for (idx_t k = 1; k <= i; ++k) {
        seq_t prod = (seq_t)j * k;                  /* k·j                        */
        sum += calc_phi(prod);                     /* φ(k·j)  */
    }
    return sum / denom;                            /* integer division           */
}
