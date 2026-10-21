/*
 * A260687  Triangular array with n-th row giving coefficients of polynomial
 *          Product_{k = 2..n} (k + n*t) for n >= 1.
 * https://oeis.org/A260687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth element (row‑major order) of the triangle with
 *            rows r = 1,2,… where row r contains the coefficients of
 *            ∏_{k=2..r} (k + r*t). In particular, a(r,c) = r^c * e_{r-1-c}
 *            (0‑based column) where e_m is the elementary symmetric sum of
 *            degree m of the set {2,…,r}
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Find row r where nth term lies (triangular indexing) */
    idx_t low  = 1;
    idx_t high = 2 * n;                     /* ensures (high-1)*high/2 >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = ( (__uint128_t)mid * (mid + 1) ) / 2;
        if (tri >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t r = low;                          /* row index (≥1)                */

    /* Triangular number of all previous rows */
    __uint128_t prev_tri = ( (__uint128_t)(r - 1) * r ) / 2;
    idx_t k = (idx_t)(n - prev_tri - 1);    /* column index 0‑based in row r */

    /* ------------------------------------------------------------
       Cache of elementary symmetric sums e_m for the set {2,…,cur_max_r}.
       e[m] stores E_m = sum over all subsets of size m of the product of
       their elements (with m = 0 => e[0] = 1).  The array grows as r grows.
       ------------------------------------------------------------ */
    static seq_t *e = NULL;                 /* indices 0…max_degree */
    static idx_t cur_max_r = 1;             /* largest element added   */

    if (e == NULL) {
        size_t init_size = 2;               /* need indices 0 and 1 */
        e = (seq_t *)calloc(init_size, sizeof(seq_t));
        if (!e) abort();
        e[0] = (seq_t)1;                   /* e[0] = 1 for empty set   */
        cur_max_r = 1;                     /* empty set corresponds to r = 1 */
    }

    /* Grow the cache if we need a larger set {2,…,r} */
    if (r > cur_max_r) {
        size_t new_size = (size_t)(r + 1);  /* indices 0 … r   */
        seq_t *tmp = (seq_t *)realloc(e, new_size * sizeof(seq_t));
        if (!tmp) abort();
        e = tmp;

        /* New slots are already zero from malloc; ensure e[0] stays 1 */
        for (idx_t i = cur_max_r + 1; i <= r; ++i)
            e[i] = (seq_t)0;

        /* Incorporate each new element x = cur_max_r+1 … r */
        for (idx_t x = cur_max_r + 1; x <= r; ++x) {
            for (idx_t i = x; i > 0; --i) { /* update e[i] = e[i] + x*e[i-1] */
                e[i] = e[i] + (seq_t)x * e[i - 1];
            }
        }
        cur_max_r = r;
    }

    /* Compute the required coefficient */
    idx_t m = r - 1 - k;                    /* degree of symmetric sum  */
    seq_t coeff = e[m];
    seq_t pow_rk = seq_pow( (seq_t)r, k );

    return coeff * pow_rk;
}
