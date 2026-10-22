/*
 * A355174  The Fuss-Catalan triangle of order 3, read by rows. Related to
 *          quartic trees.
 * https://oeis.org/A355174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the Fuss–Catalan triangle of
 *            order 3 (row 0 = [1], row 1 = [0,1], row n (n>=2) is obtained by
 *            duplicating the last element of row n‑1 and taking the
 *            cumulative sum 3 times). For a given index n, let r be the row
 *            number (0‑based) and k the column in that row. Then T(...
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* determine row r and column k for index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    idx_t cum_before = r * (r + 1) / 2;
    idx_t k = n - cum_before;          /* 0 ≤ k ≤ r */

    /* special cases */
    if (r == 0 && k == 0) {
        return (seq_t)1;
    }
    if (k == 0) {
        return (seq_t)0;
    }
    if (k == 1) {
        return (seq_t)1;
    }

    /* k >= 2: compute using the closed form */
    idx_t km1 = k - 1;
    idx_t km2 = k - 2;

    seq_t mult = (seq_t)3 * (seq_t)(r - k) + (seq_t)4;   /* 3*(r-k)+4  */
    seq_t B    = (seq_t)3 * (seq_t)r + (seq_t)1;          /* 3*r + 1   */

    idx_t binom_n = (idx_t)B + km2;   /* 3*r + 1 + (k-2) = 3*r + k - 1 */
    idx_t binom_k = km2;              /* k-2 */

    seq_t binom_val = binomial(binom_n, binom_k);           /* binomial(3*r + k - 2, k-2) */

    seq_t numerator = mult * binom_val;                     /* multiply 3*(r-k)+4 with binomial */
    seq_t result    = numerator / (seq_t)km1;               /* divide by (k-1); division is exact */

    return result;
}
