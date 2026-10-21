/*
 * A250355  Number of length 6 arrays x(i), i=1..6 with x(i) in i..i+n and no
 *          value appearing more than 2 times.
 * https://oeis.org/A250355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 - 5*n^4 + 55*n^3 + 25*n^2 - 61*n + 98 for n>3
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    /* explicit values for the first three indices */
    if (n == 1u) return (seq_t)64;
    if (n == 2u) return (seq_t)622;
    if (n == 3u) return (seq_t)3349;

    /* compute the 6th‑degree polynomial via Horner's rule using a 128‑bit temporary */
    __int128 v = 1;
    __int128 nn = ( __int128 ) n;
    v = v * nn + 6;   /* + 6*n */
    v = v * nn - 5;   /* - 5*n^2 */
    v = v * nn + 55;  /* + 55*n^3 */
    v = v * nn + 25;  /* + 25*n^4 */
    v = v * nn - 61;  /* - 61*n^5 */
    v = v * nn + 98;  /* + 98*n^6 */

    return (seq_t) v;
}
