/*
 * A350454  Number T(n,k) of endofunctions on [n] with exactly k fixed points,
 *          none of which are isolated; triangle T(n,k), n >= 0, 0 <= k <=
 *          n/2, read by rows.
 * https://oeis.org/A350454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the (n+1)-th term of the triangle T(n,k) of
 *            endofunctions on [n] with exactly k non‑isolated fixed points,
 *            read by rows. T(r,k) = C(r,k) * Σ_{i=0..k} (-1)^i * C(k,i) *
 *            (r-1-i)^(r-k)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 35, u64 n <= 80, u128 n <= 209,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes the required helper functions for binomial coefficients and
   integer powers. */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/seq_pow.h"    /* seq_t seq_pow(seq_t base, idx_t exp); */

seq_t a(idx_t n)
{
    /* Find the row `row` such that the cumulative number of terms up to
       the previous row is <= n and the cumulative number up to this row
       is > n.  The length of row r is floor(r/2)+1. */
    idx_t row = 0;
    idx_t cum = 0;
    while (1) {
        idx_t len = row / 2 + 1;          /* number of terms in this row   */
        if (cum + len > n) break;         /* n lies in this row            */
        cum += len;
        ++row;
    }
    /* Position within the row (0‑based). */
    idx_t pos = n - cum;
    idx_t k   = pos;                       /* column index in the triangle  */

    /* Compute T(row,k).  Here m = row-k is the number of non‑fixed points. */
    idx_t m = row - k;
    seq_t sum = 0;
    for (idx_t i = 0; i <= k; ++i) {
        seq_t term = binomial(k, i) *
                     seq_pow((seq_t)(row - 1 - i), m);
        if (i & 1)
            sum -= term;                  /* subtract if i is odd */
        else
            sum += term;                  /* add if i is even     */
    }
    return binomial(row, k) * sum;
}
