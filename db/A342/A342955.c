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
 * Range:     u8 n <= 10010, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 107 terms through n = 9505 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute T(i, j): digitwise minimum of the two decimal numbers */
static seq_t min_digits(idx_t i, idx_t j)
{
    seq_t result = 0;
    seq_t pow10   = 1;
    while (i > 0 || j > 0) {
        unsigned d1 = (unsigned)(i % 10);
        unsigned d2 = (unsigned)(j % 10);
        unsigned d  = d1 < d2 ? d1 : d2;
        result += (seq_t)d * pow10;
        pow10   *= 10;
        i      /= 10;
        j      /= 10;
    }
    return result;
}

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
