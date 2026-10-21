/*
 * A276588  Square array A(row,col) = Sum_{k=0..row}
 *          binomial(row,k)*(1+col+k)!, read by descending antidiagonals as
 *          A(0,0), A(0,1), A(1,0), A(0,2), A(1,1), A(2,0), ...
 * https://oeis.org/A276588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 32, u32 n <= 77, u64 n <= 209,
 *            u128 n <= 565, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276588 – a(n) = Sum_{k=0..row} binomial(row,k)*(1+col+k)!,
   read by antidiagonals. */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal d such that T = d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d = 0;
    /* Use unsigned arithmetic; the values are small (< 200 for n ≤ 10000). */
    while ((d * (d + 1) / 2) <= n) {
        ++d;
    }
    --d;                         /* now d satisfies T ≤ n */
    idx_t t = d * (d + 1) / 2;
    idx_t offset = n - t;        /* 0‑based index on this diagonal */

    /* Row increases from 0 to d along the diagonal; the corresponding column is d-offset. */
    idx_t row = offset;
    idx_t col = d - offset;

    seq_t sum = 0;
    for (idx_t k = 0; k <= row; ++k) {
        seq_t bin = binomial(row, k);              /* binomial(row,k) */
        seq_t fac = factorial(1 + col + k);        /* (1 + col + k)! */
        sum += bin * fac;                          /* accumulate */
    }
    return sum;
}
