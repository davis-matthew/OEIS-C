/*
 * A367824  Array read by ascending antidiagonals: A(n, k) is the numerator of
 *          (R(n) - k)/(n + k), where R(n) is the digit reversal of n, with
 *          A(0, 0) = 1.
 * https://oeis.org/A367824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, frac, look, tabl
 * Formula:   a(n) = numerator of (rev(n) − k)/(n + k) * where the (n,k) index
 *            is obtained by reading the array “A(n,k)” * by ascending
 *            antidiagonals – for each d = n + k the entries * are taken in
 *            the order (d,0),(d‑1,1),…,(0,d)
 * Range:     i8 n <= 5355, i16 n <= 9677, i32 n <= 9677, i64 n <= 9677,
 *            i128 n <= 9677, big any n
 * Helpers:   rev, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/rev.h"          /* reverse decimal digits of an idx_t */
#include "../utils/seq_gcd.h"      /* seq_gcd(seq_t a, seq_t b) */

/* Compute the (n,k) coordinates from the linear index i.  */
static void coords_from_index(idx_t i, idx_t *n_coord, idx_t *k_coord) {
    /* Find the diagonal d such that d*(d+1)/2 <= i < (d+1)*(d+2)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= i)
        ++d;

    idx_t offset = i - d * (d + 1) / 2;
    *n_coord = d - offset;   /* decreasing n along the diagonal   */
    *k_coord = offset;       /* increasing k along the diagonal   */
}

seq_t a(idx_t n) {
    if (n == 0)                 /* A(0,0) = 1 by definition */
        return (seq_t)1;

    idx_t r, c;                 /* (n,k) coordinates */
    coords_from_index(n, &r, &c);

    idx_t rev_r = rev(r);                    /* rev(r) fits in idx_t   */
    seq_t num = (seq_t)rev_r - (seq_t)c;     /* numerator               */
    seq_t den = (seq_t)r + (seq_t)c;         /* denominator             */

    seq_t g = seq_gcd(num >= 0 ? num : -num, den);
    return num / g;
}
