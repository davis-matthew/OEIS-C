/*
 * A325027  Triangle read by rows: T(n,k), 0 <= k < n, is the least m
 *          minimizing the number of intervals [a,a+1) or [ma,m(a+1)) that
 *          must be XORed together to form the interval [k,n).
 * https://oeis.org/A325027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(row, col) where T(row,col) is the least m minimizing
 *            F(row,col,m); indices start at 1
 * Range:     u8 n <= 2480, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9842 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes */
#include <stdint.h>
#include <stdbool.h>

/* Compute the triangle value T(n,k) = least m minimizing F(n,k,m) */
static seq_t triangle_min_m(idx_t n, idx_t k)
{
    /* If n > (k+1)^2, property 4 says the answer is n */
    seq_t kp1 = (seq_t)(k + 1);
    seq_t sq  = kp1 * kp1;
    if ((seq_t)n > sq)
        return (seq_t)n;

    idx_t best_m = 0;
    seq_t best_F = 0;
    bool first = true;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sn   = (seq_t)n;
        seq_t sk   = (seq_t)k;
        seq_t sm   = (seq_t)m;

        /* u = ceil(n/m - 1/2) = floor((2n + m - 1) / (2m)) */
        seq_t u = ( (seq_t)2 * sn + sm - (seq_t)1 ) / ( (seq_t)2 * sm );
        /* v = floor(k/m + 1/2) = floor((2k + m) / (2m)) */
        seq_t v = ( (seq_t)2 * sk + sm ) / ( (seq_t)2 * sm );

        seq_t prod_u = u * sm;                 /* u*m */
        seq_t prod_v = v * sm;                 /* v*m */

        seq_t diff1 = (prod_u > sn) ? (prod_u - sn) : (sn - prod_u);
        seq_t diff2 = (prod_v > sk) ? (prod_v - sk) : (sk - prod_v);

        seq_t F = u - v + diff1 + diff2;

        if (first || F < best_F || (F == best_F && m < best_m)) {
            best_F = F;
            best_m = m;
            first = false;
        }
    }

    return (seq_t)best_m;
}

/* Main sequence function: returns the nth term (offset 1) */
seq_t a(idx_t n)
{
    /* Determine row and column from the serial index n */
    idx_t row = 1;
    while (row * (row + 1) / 2 < n)
        ++row;

    idx_t prev_rows = (row - 1) * row / 2;
    idx_t k = n - prev_rows - 1;   /* 0‑based column */

    return triangle_min_m(row, k);
}
