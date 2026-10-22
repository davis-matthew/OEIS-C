/*
 * A382310  Array read by ascending antidiagonals: A(n,m) is the squared
 *          distance between the roots of the 2nd degree equations z^2 +- n*z
 *          + m = 0 on the complex plane.
 * https://oeis.org/A382310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A382310.
   a(n) = |i^2 - 4*j| where (i,j) are the coordinates of the
   element at index n in the two‑dimensional array
   A(i,j) = |i^2 - 4*j| read by ascending antidiagonals.
   The mapping from n to (i,j) is:
       let s be the smallest integer with (s+1)(s+2)/2 > n
       let k = n - s(s+1)/2          // offset in diagonal s
       i = s - k
       j = k
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find diagonal s such that (s+1)(s+2)/2 > n */
    idx_t s = 0;
    while ((seq_t)(s + 1) * (s + 2) / 2 <= n) {
        ++s;
    }

    /* Number of terms before diagonal s */
    idx_t t_prev = s * (s + 1) / 2;

    /* Offset within the diagonal */
    idx_t k = n - t_prev;

    /* Coordinates (i,j) on the array */
    idx_t i = s - k;
    idx_t j = k;

    /* Compute |i^2 - 4*j| using seq_t arithmetic */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t val2 = ii * ii;
    seq_t val4j = (seq_t)4 * jj;

    if (val2 >= val4j) {
        return val2 - val4j;
    } else {
        return val4j - val2;
    }
}
