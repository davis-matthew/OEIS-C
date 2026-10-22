/*
 * A350161  Square array T(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} (-1)^(j+1) *
 *          floor(n/(2*j-1))^k.
 * https://oeis.org/A350161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where T(r, c) = Σ_{j=1..r} (-1)^(j+1) * ⌊r /
 *            (2*j-1)⌋^c and (row, col) are obtained by reading the square
 *            array by antidiagonals downwards
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/big_pow.h"

/* Return a(n) for n >= 1.  n is an idx_t (unsigned 64‑bit);  the
   value is a seq_t (unsigned, possibly arbitrary‑precision). */
seq_t a(idx_t n)
{
    /* Find the antidiagonal index d such that d*(d+1)/2 >= n.  */
    idx_t d = (idx_t)((sqrtl((long double)n * 8.0L + 1.0L) - 1.0L) / 2.0L);
    /* Correct possible rounding errors */
    while ((unsigned __int128)d * (d + 1) / 2 < n) ++d;
    while (d > 0 && (unsigned __int128)(d - 1) * d / 2 >= n) --d;

    /* Number of terms before this diagonal */
    idx_t prev = (d - 1) * d / 2;

    /* Position within the diagonal (0‑based) */
    idx_t pos = n - prev - 1;

    /* Coordinates in the array:
         row = pos + 1        (rows increase from 1)
         col = d - pos        (columns decrease from d)   */
    idx_t row = pos + 1;
    idx_t col = d - pos;

    /* Compute the alternating sum */
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;
    for (idx_t j = 1; j <= row; ++j) {
        idx_t denom = 2 * j - 1;
        idx_t f = row / denom;                 /* floor(row / (2*j-1)) */
        seq_t val = big_pow((seq_t)f, (seq_t)col);  /* f^col as seq_t */

        if (j & 1U)  /* odd -> sign + */
            pos_sum += val;
        else         /* even -> sign - */
            neg_sum += val;
    }

    return pos_sum - neg_sum;  /* the result is non‑negative */
}
