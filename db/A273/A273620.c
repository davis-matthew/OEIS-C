/*
 * A273620  Table read by ascending antidiagonals: T(n, k) = floor(sqrt(k) *
 *          floor(n/sqrt(k) + 1)), for n >= 1, k >= 1.
 * https://oeis.org/A273620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the antidiagonal coordinates of
 *            (n)
 * Range:     u8 n <= 16, u16 n <= 6136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* T(n,k) = floor( sqrt(k) * floor( n / sqrt(k) + 1 ) ) */

#include "../utils/sqrt_floor.h"   /* provides sqrt_floor(seq_t) */

/* compute a single table entry T(n,k) for 1‑based indices n,k */
static inline seq_t table_T(idx_t n, idx_t k)
{
    /* a = floor( n^2 / k )  (integer division) */
    seq_t a = (seq_t)n * (seq_t)n / (seq_t)k;

    /* m = floor( sqrt(a) ) + 1  =  floor( n / sqrt(k) ) + 1 */
    seq_t m = sqrt_floor(a) + 1;

    /* product = k * m^2 */
    seq_t prod = (seq_t)k * m * m;

    /* T = floor( sqrt(product) ) */
    return sqrt_floor(prod);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* find the antidiagonal 't' such that t*(t+1)/2 >= n */
    idx_t t = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++t;
        cum = (t * (t + 1)) / 2;
    }

    /* position on the diagonal (1‑based) */
    idx_t prev_cum = ( (t - 1) * t ) / 2;
    idx_t pos = n - prev_cum;          /* 1 ≤ pos ≤ t */

    /* compute (i,j) coordinates of the table entry */
    idx_t i = t + 1 - pos;  /* row index, 1‑based */
    idx_t j = pos;          /* column index, 1‑based */

    return table_T(i, j);
}
