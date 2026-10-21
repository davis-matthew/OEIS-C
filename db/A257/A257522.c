/*
 * A257522  Table T(i,j) = denominator of (1/i + 1/j) = i*j/gcd(i*j,i+j) read
 *          by antidiagonals (i >= 1, j >= 1).
 * https://oeis.org/A257522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = i*j/gcd(i*j,i+j) where (i,j) satisfy * n =
 *            (i+j-2)*(i+j-1)/2 + i (1 ≤ i,j)
 * Range:     u8 n <= 508, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the diagonal sum s = i + j: the minimal s with
       C(s) = (s-1)*s/2 >= n  */
    idx_t s = 2;
    while (((s - 1) * s) / 2 < n)
        ++s;

    /* Number of terms before this diagonal */
    idx_t prev = ((s - 2) * (s - 1)) / 2;
    /* Position within the diagonal (0‑based) */
    idx_t k = n - prev - 1;
    /* Coordinates (i,j) */
    idx_t i = k + 1;
    idx_t j = s - i;

    /* Compute denominator of 1/i + 1/j = i*j/(i*j) + ... */
    seq_t num = (seq_t)i * j;         /* i * j  */
    seq_t den = (seq_t)(i + j);       /* i + j  */
    seq_t g   = gcd(num, den);        /* gcd(i*j, i+j) */
    return num / g;                   /* i*j/gcd(i*j,i+j) */
}
