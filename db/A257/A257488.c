/*
 * A257488  Triangle, read by rows, T(n,k) = k*Sum_{i=0..n-k}
 *          C(2*i+2*k,i)*C(n-i-1,k-1)/(i+k) for 1 <= k <= n.
 * https://oeis.org/A257488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A257488
   Triangle T(n,k) = k * sum_{i=0..n-k} C(2*i+2*k,i) * C(n-i-1,k-1) / (i+k)
   Sequence a(n) is the triangle read by rows (row‑major order) and
   offset 1.  For n ≥ 1, a(n) = T(r,k) where r is the smallest integer
   with r(r+1)/2 ≥ n and k = n - (r-1)r/2. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* Return the n‑th term of A257488 (a(1)=1). */
seq_t a(idx_t n)
{
    /* Guard against n=0 (undefined).  The offset is 1. */
    if (n == 0) return 0;

    /* Find the triangle row r containing the nth element. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) r++;

    /* Columns are indexed from 1 to r. */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t k = n - prev_sum;          /* 1 ≤ k ≤ r */

    /* Compute T(r,k) */
    seq_t total = 0;
    idx_t max_i = r - k;             /* i goes from 0 to r-k */

    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t c1 = binom(2 * i + 2 * k, i);
        seq_t c2 = binom(r - i - 1, k - 1);
        seq_t denom = (seq_t)i + (seq_t)k;
        seq_t term = ((seq_t)k * c1 * c2) / denom;
        total = total + term;
    }
    return total;
}
