/*
 * A253026  T(n,k) = Number of n X k nonnegative integer arrays with upper
 *          left 0 and lower right its king-move distance away minus 1 and
 *          every value within 1 of its king move distance from the upper left
 *          and every value increasing by 0 or 1 with every step right,
 *          diagonally se or down.
 * https://oeis.org/A253026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the n‑th pair of nonnegative *
 *            integers enumerated antidiagonally (OEIS table T(n,k) with
 *            n>=1,k>=1). * Here T(n,k) = (n-k)*4^(k-1)+(4^(k-1)-1)/3 for
 *            n>=k>=1 and T(n,k)=T(k,n). * The implementation converts the
 *            0‑based indices returned by * antidiagonal_coords to 1‑b...
 * Range:     u8 n <= 40, u16 n <= 161, u32 n <= 577, u64 n <= 2177,
 *            u128 n <= 8449, big any n
 * Helpers:   antidiagonal_coords, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/antidiagonal_coords.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety: N is 1‑based */

    idx_t r, c;                               /* 0‑based row/col from helper */
    antidiagonal_coords(n - 1, &r, &c);

    /* convert to 1‑based indices */
    r++; c++;

    /* ensure r <= c for the formula */
    if (r > c) {
        idx_t t = r; r = c; c = t;
    }

    seq_t pow4_val = pow_int((seq_t)4, r - 1); /* 4^(r-1) */
    seq_t diff      = (seq_t)(c - r);         /* non‑negative difference */

    seq_t term1 = diff * pow4_val;
    seq_t term2 = (pow4_val - 1) / 3;         /* exact division */

    return term1 + term2;
}
