/*
 * A386824  Triangle read by rows: T(n,k) = denominator((n^2 - k^2)/(n^2 +
 *          k^2)), where 0 <= k < n.
 * https://oeis.org/A386824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, look, tabl, changed
 * Formula:   a(n) = denominator((nrow^2 - k^2)/(nrow^2 + k^2)), where
 *            (nrow,k) are the 1‑based row and 0‑based column indices of the
 *            n‑th element of the triangle read row‑by‑row
 * Range:     u8 n <= 77, u16 n <= 8197, u32 n <= 8197, u64 n <= 8197,
 *            u128 n <= 8197, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 0 */

    /* Find the row r such that r(r+1)/2 ≥ n.  */
    idx_t r = 1;
    idx_t cum = 1;                        /* sum of terms up to row r */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Compute column k (0‑based). */
    idx_t prev = cum - r;                 /* sum up to previous row */
    idx_t k = n - prev - 1;

    /* Work in seq_t arithmetic. */
    seq_t R = (seq_t)r;
    seq_t K = (seq_t)k;

    seq_t num = R * R - K * K;            /* numerator of the fraction  */
    seq_t den = R * R + K * K;            /* denominator of the fraction  */

    seq_t g = gcd(num, den);              /* reduce fraction            */
    return den / g;                       /* return reduced denominator */
}
