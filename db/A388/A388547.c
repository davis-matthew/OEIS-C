/*
 * A388547  Triangle read by rows: T(n,k) is the number of length n binary
 *          words which are not all zero in which 1's occur in blocks of at
 *          least k.
 * https://oeis.org/A388547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{i=1..⌊(n+1)/(k+1)⌋} binomial(n+1-(k-1)*i, 2*i)
 *            a(n) is the triangle read by rows: a(1)=T(1,1), a(2)=T(2,1),
 *            a(3)=T(2,2), a(4)=T(3,1), a(5)=T(3,2), a(6)=T(3,3), … The
 *            function below implements this definition exactly
 * Range:     u8 n <= 21, u16 n <= 136, u32 n <= 528, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n<1 the value is undefined, return 0 */
    if (n < 1U)
        return (seq_t)0;

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t low  = 1;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t tri = (mid * (mid + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;

    /* Column index (1‑based) within this row */
    idx_t start = ((row - 1) * row) / 2 + 1;  /* first element of the row */
    idx_t col   = n - start + 1;

    /* Compute T(row, col) */
    idx_t max_i = (row + 1) / (col + 1);
    seq_t sum   = (seq_t)0;
    for (idx_t i = 1; i <= max_i; ++i) {
        /* n' = row + 1 - (col-1)*i  (rationally the same as the
           formula's n+1-(k-1)*i) */
        idx_t np = row + 1 - (col - 1) * i;
        seq_t term = binom(np, 2 * i);
        sum += term;
    }
    return sum;
}
