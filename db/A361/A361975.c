/*
 * A361975  (2,1)-block array, B(2,1), of the natural number array (A000027),
 *          read by descending antidiagonals.
 * https://oeis.org/A361975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 4*i - 1 + (2*i + j - 2)^2 where (i,j) are the coordinates
 *            of the n‑th entry in the (2,1)-block array B(2,1) read by
 *            descending antidiagonals. Obtain (i,j) by: let s be the smallest
 *            integer with s*(s-1)/2 >= n; t_before = (s-2)*(s-1)/2; offset =
 *            n - t_before - 1; j = (s-1) - offset; i = s - j
 * Range:     u8 n <= 35, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* index is 1‑based */

    /* Find row sum s such that (s-1)*s/2 >= n */
    idx_t s = 1;
    while ((s * (s - 1)) / 2 < n)
        ++s;

    idx_t t_before = 0;
    if (s >= 2)
        t_before = ((s - 2) * (s - 1)) / 2;

    idx_t offset = n - t_before - 1;
    idx_t j = (s - 1) - offset;
    idx_t i = s - j;

    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t t = 2 * ii + jj - 2;           /* 2*i + j - 2   */
    seq_t value = 4 * ii - 1 + t * t;     /* 4*i -1 + (t)^2 */

    return value;
}
