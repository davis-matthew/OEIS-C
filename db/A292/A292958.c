/*
 * A292958  Rectangular array by antidiagonals: T(n,m) = rank of n*(r+m) when
 *          all the numbers k*(r+h), where r = sqrt(5), k>=1, h>=0, are
 *          jointly ranked.
 * https://oeis.org/A292958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the rank of the element corresponding to the
 *            (i,j)antidiagonal coordinates of n in the array T(i,j) defined
 *            by OEIS A292958: T(i,j) = rank of i*(sqrt(5)+j) among all
 *            k*(sqrt(5)+h) with k>=1, h>=0. The formula for the rank is:
 *            rank(i,j) = 1 + Σ_{k=1}^{kmax} (floor((i*j + (i-k)*sqrt(5)) /
 *            k...
 * Range:     u8 n <= 126, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   floor_div_sqrt5, less_than_sqrt5, rank_n_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_div_sqrt5.h"
#include "../utils/less_than_sqrt5.h"
#include "../utils/rank_n_m.h"

/* Convert the linear index n (1‑based) to matrix coordinates (i,j) (i>=1,j>=0)
   following the antidiagonal reading.                                                       */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t lo = 1, hi = 1;
    /* grow hi until triangular(hi) >= n */
    while ((__int128)hi * (hi + 1) / 2 < n) {
        hi <<= 1;
    }
    /* binary search for minimal d with d*(d+1)/2 >= n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo;
    __int128 prev = (__int128)d * (d - 1) / 2;
    idx_t offset = n - (idx_t)prev - 1;
    *i = offset + 1;
    *j = d - offset - 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset safety */
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return rank_n_m(i, j);
}
