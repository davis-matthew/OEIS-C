/*
 * A337406  Number of chiral pairs of colorings of the edges of a cube (or
 *          regular octahedron) using n or fewer colors.
 * https://oeis.org/A337406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * n^2 * (n+1) * (n^8 + n^6 - 2*n^4 + 8) / 48
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;               /* out of range */
    seq_t sn = (seq_t)n;                      /* n */
    seq_t n2 = sn * sn;                       /* n^2 */
    seq_t n4 = n2 * n2;                       /* n^4 */
    seq_t n6 = n4 * n2;                       /* n^6 */
    seq_t n8 = n4 * n4;                       /* n^8 */

    seq_t term = n8 + n6 - (seq_t)2 * n4 + (seq_t)8; /* n^8 + n^6 - 2*n^4 + 8 */

    seq_t n_minus1 = (seq_t)(n - 1);
    seq_t n_plus1 = (seq_t)(n + 1);

    seq_t num = n_minus1 * n2;                /* (n-1)*n^2 */
    num = num * n_plus1;                      /* * (n+1) */
    num = num * term;                         /* * term */

    return num / (seq_t)48;                   /* divide by 48 */
}
