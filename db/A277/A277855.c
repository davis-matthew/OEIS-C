/*
 * A277855  Irregular triangle read by rows: T(n,k) is the maximum length of
 *          the longest common subsequence of k distinct permutations of n
 *          items with n>=1 and 1<=k<=n!
 * https://oeis.org/A277855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th entry of the irregular triangle.
 * For a given index n, let r be the unique row such that
 *   Σ_{i=1}^{r-1} i! < n ≤ Σ_{i=1}^r i!
 * Let k = n – Σ_{i=1}^{r-1} i!   (1 ≤ k ≤ r!).
 * Then
 *   if k == 1      -> a(n) = r;
 *   else
 *     a(n) = r – j   where j is the unique integer 1 ≤ j ≤ r with  
 *                     binomial(r, r-j+1)*(j-1)! + 1 ≤ k ≤ binomial(r, r-j)*j!.
 */
#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined offset, but guard anyway */

    /* Find the row r containing the n-th term */
    idx_t       row = 1;
    seq_t       f   = factorial(row);   /* r! for current row */
    seq_t       cum = 0;                /* Σ_{i=1}^{row-1} i! */

    seq_t target = (seq_t)n;
    while (cum + f < target) {
        cum += f;                /* add previous row factorial */
        ++row;                   /* move to next row */
        f = factorial(row);       /* compute new r! */
    }

    /* Position k of the term within row r (1-indexed) */
    idx_t k = n - (idx_t)cum;   /* cum fits into idx_t for allowed n */

    if (k == 1) return (seq_t)row;   /* first permutation */

    /* Find the appropriate j for the remaining k */
    for (idx_t j = 1; j <= row; ++j) {
        seq_t lower = binomial(row, row - j + 1) * factorial(j - 1) + 1;
        seq_t upper = binomial(row, row - j)     * factorial(j);
        if ((seq_t)k >= lower && (seq_t)k <= upper) {
            return (seq_t)(row - j);
        }
    }

    /* Should never reach here */
    return 0;
}
