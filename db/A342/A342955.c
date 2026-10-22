/*
 * A342955  Array T(n,k), n, k >= 0, read by antidiagonals; the i-th decimal
 *          digit of T(n, k) is the smallest of the i-th digits of n and of k.
 * https://oeis.org/A342955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base, easy, look
 * Formula:   a(n) = digitwise minimum of the antidiagonal coordinates (i, j)
 *            derived from n, i.e. T(i, j) where T(i, j) keeps the least
 *            decimal digit of i or j at each position
 * Range:     u8 n <= 9505, u16 n <= 9505, u32 n <= 9505, u64 n <= 9505,
 *            u128 n <= 9505, big any n
 * Helpers:   min_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/min_digits.h"

/* a(n) = T(i, j) where (i, j) are the coordinates obtained by reading the
   infinite array T(k, l) along antidiagonals. */
seq_t a(idx_t n)
{
    /* Find diagonal sum s such that
       prev = s*(s+1)/2 <= n < (s+1)*(s+2)/2            */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) ++s;

    idx_t prev   = s * (s + 1) / 2;
    idx_t offset = n - prev;           /* offset within diagonal   */
    idx_t i = offset;                  /* row coordinate           */
    idx_t j = s - offset;              /* column coordinate        */

    return min_digits(i, j);
}
