/*
 * A257852  Array A read by upward antidiagonals in which the entry A(n,k) in
 *          row n and column k is defined by A(n,k) = (2^n*(6*k - 3 -
 *          2*(-1)^n) - 1)/3, n,k >= 1.
 * https://oeis.org/A257852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^row * (6*col - 3 - 2*(-1)^row) - 1)/3, where (row,col)
 *            are the coordinates found by reading the infinite array A upward
 *            antidiagonally (first the 1‑st diagonal, then the 2‑nd, etc.)
 * Range:     u8 n <= 28, u16 n <= 120, u32 n <= 497, u64 n <= 2017,
 *            u128 n <= 8129, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* n is 1‑based index in the sequence */
    idx_t remaining = n;
    idx_t d = 2;                     /* current diagonal number (≥2) */

    /* Find the diagonal that contains the n‑th term
       (stop when remaining fits in this diagonal)          */
    while (remaining > d - 1) {
        remaining -= (d - 1);
        ++d;
    }

    idx_t pos = remaining;           /* position in diagonal (1‑based) */
    idx_t row = d - pos;             /* n‑coordinate of the cell      */
    idx_t col = pos;                 /* k‑coordinate of the cell      */

    /* Compute A(row,col) by the explicit formula */
    int sign = (row & 1) ? -1 : 1;   /* (-1)^row */

    seq_t t = 6 * (seq_t)col - 3;    /* start with 6*col - 3          */
    if (sign == -1)
        t += 2;                      /* if n odd: add 2                */
    else
        t -= 2;                      /* if n even: subtract 2           */

    /* A(row,col) = (2^row * t - 1) / 3 */
    seq_t val = (pow2(row) * t - 1) / 3;
    return val;
}
