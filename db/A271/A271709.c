/*
 * A271709  Table T(n,k) = 2^n + 2^k read by antidiagonals.
 * https://oeis.org/A271709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (n)-th term of the table T(i,j)=2^i+2^j * read by
 *            antidiagonals (i+j constant), i decreasing from the sum. * For
 *            index n, find the diagonal s=i+j such that * s*(s+1)/2 ≤ n <
 *            (s+1)*(s+2)/2, * then let d = n - s*(s+1)/2, so that i = s - d,
 *            j = d. * a(n) = 2^(s-d) + 2^d
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 8255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t find_diagonal(idx_t n) {
    /* Find the largest s with s*(s+1)/2 <= n (triangle number). */
    idx_t low = 0, high = 1;
    /* Exponential search for an upper bound. */
    while (1) {
        unsigned __int128 t = (unsigned __int128)high * (high + 1) / 2;
        if (t > n) break;
        if (high == UINT64_MAX) { high = UINT64_MAX; break; }
        high <<= 1;
    }
    /* Binary search between low and high. */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n) low = mid;
        else high = mid;
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t s = find_diagonal(n);                       /* diagonal number */
    unsigned __int128 tri128 = (unsigned __int128)s * (s + 1) / 2;
    idx_t tri = (idx_t)tri128;                        /* triangular number s(s+1)/2 */
    idx_t d = n - tri;                                /* offset along diagonal */
    idx_t i = s - d;                                  /* row index */
    idx_t j = d;                                      /* column index */
    seq_t term = pow2(i) + pow2(j);                   /* 2^i + 2^j */
    return term;
}
