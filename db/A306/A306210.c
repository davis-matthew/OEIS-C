/*
 * A306210  T(n,k) = binomial(n + k, n) - binomial(n + floor(k/2), n) + 1,
 *          square array read by descending antidiagonals (n >= 0, k >= 0).
 * https://oeis.org/A306210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   * a(n) = T(i,j) = C(i+j,i) - C(i+⌊j/2⌋,i) + 1 * where (i,j) are
 *            the unique non‑negative integers satisfying * i+j = s, s =
 *            ⌊(√(8n+1) – 1)/2⌋, * and i = n – s(s+1)/2
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Compute the n-th term of A306210 */
seq_t
a(idx_t n)
{
    /* Find s such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    /* Use 128‑bit arithmetic to avoid overflow for large n */
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n)
        ++s;

    /* offset within the diagonal s */
    idx_t offset = (idx_t)((unsigned __int128)n -
                           (unsigned __int128)s * (s + 1) / 2);

    idx_t i = offset;           /* n‑coordinate along the diagonal */
    idx_t j = s - offset;       /* k‑coordinate */

    seq_t b1 = binomial(i + j, i);                /* C(i+j,i)   */
    seq_t b2 = binomial(i + (j >> 1), i);        /* C(i+⌊j/2⌋,i)  */

    return b1 - b2 + 1;
}
