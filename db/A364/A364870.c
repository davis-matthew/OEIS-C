/*
 * A364870  Array read by ascending antidiagonals: A(n, k) = (n + k)^n, with k
 *          >= 0.
 * https://oeis.org/A364870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = d^(d - (n - d*(d+1)/2)) where d =
 *            floor((sqrt(8*n+1)-1)/2) is the diagonal index and d - (n -
 *            d*(d+1)/2) is the row index within that diagonal
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

/* Integer square‑root based on binary search (no floating point used). */
static idx_t diag_index(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* Expand hi until (hi*(hi+1))/2 > n, guarding against overflow. */
    while (((unsigned __int128)hi * (hi + 1)) / 2 <= n) {
        hi <<= 1;
        if (hi == 0) break;                /* overflow guard */
    }
    /* Binary search for the largest hi such that hi*(hi+1)/2 <= n. */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n) lo = mid;
        else          hi = mid - 1;
    }
    return lo;                               /* this is d */
}

seq_t a(idx_t n)
{
    idx_t d = diag_index(n);                 /* diagonal number */

    unsigned __int128 tri = (unsigned __int128)d * (d + 1) / 2; /* terms before diag d */
    idx_t offset = n - (idx_t)tri;           /* position inside the diagonal */
    idx_t i = d - offset;                    /* row index in the array */

    /* (n+k)^n = d^i for the coordinates (i, d-i). */
    return seq_pow((seq_t)d, i);
}
