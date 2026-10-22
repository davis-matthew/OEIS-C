/*
 * A338022  Triangle T(n,m) = Sum_{i=0..n-m}
 *          C(2*m-1,n-i-1)*C(n-i-1,i)*C(n-i-1,n-m-i)/(2*m-1).
 * https://oeis.org/A338022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 42, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A338022:  Triangle T(n,m) = Σ_{i=0..n-m} C(2m-1, n-i-1)·C(n-i-1,i)·C(n-i-1,n-m-i)/(2m-1),
    indexed by the antidiagonal ordering of the triangle.
    a(n) returns the nth term of the sequence with a(1)=1. */
#include <math.h>
#include <stdbool.h>
#include "../utils/binomial.h"       /* binomial(idx_t n, idx_t k) returns seq_t */

/* Compute one entry of the triangle.  The result fits in seq_t. */
static seq_t
triangle_T(idx_t n, idx_t m)
{
    seq_t sum   = 0;
    idx_t max_i = (n > m) ? (n - m) : 0;
    seq_t denom = (seq_t)(2 * m - 1);

    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t nk1 = n - i - 1;                    /* n‑i‑1 */
        seq_t term1 = binomial(2 * m - 1, nk1);   /* C(2m-1, n-i-1) */
        seq_t term2 = binomial(nk1, i);           /* C(n-i-1, i)    */
        seq_t term3 = binomial(nk1, n - m - i);   /* C(n-i-1, n-m-i)*/
        seq_t prod  = term1 * term2;
        prod *= term3;
        prod /= denom;                           /* exact division */
        sum += prod;
    }
    return sum;
}

/* Return the n‑th term of A338022 (n ≥ 1). */
seq_t
a(idx_t n)
{
    /* Find the row k such that k*(k+1)/2 ≥ n.  */
    long double v = sqrtl((long double)1 + 8.0L * (long double)n);
    idx_t k = (idx_t)((v - 1.0L) / 2.0L);          /* floor estimate */

    /* Adjust k upward if necessary.  */
    while (((__int128)k * (k + 1)) / 2 < n) ++k;
    /* Adjust k downward if it overshoots.  */
    while (k > 0 && (((__int128)(k - 1) * k) / 2) >= n) --k;

    /* Column index m (1‑based) inside row k.  */
    idx_t prev = (idx_t)((__int128)(k - 1) * k / 2);
    idx_t m    = n - prev;   /* 1 ≤ m ≤ k */

    return triangle_T(k, m);
}
