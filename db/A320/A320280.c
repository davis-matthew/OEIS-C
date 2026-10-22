/*
 * A320280  Triangle T(n, k) = Sum_{i=1..n} Stirling2(n,i) *
 *          abs(Stirling1(i-1,k-1)), n >= 1, 1 <= k <= n.
 * https://oeis.org/A320280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle T(n,k) in row‑major
 *            order, where T(n,k) = Σ_{i=1..n} Stirling2(n,i) *
 *            |Stirling1(i‑1,k‑1)|. (Stirling numbers of the second kind S2
 *            and the signless Stirling numbers of the first kind S1 are
 *            used.)
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 79, u64 n <= 192,
 *            u128 n <= 529, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence offset starts at 1 */

    /* Find row r and column c in row‑major enumeration (1‑based). */
    idx_t r = 1;
    while (((r * (r + 1)) / 2) < n) ++r;
    idx_t c = n - ((r - 1) * r) / 2;          /* 1‑based column in row r */
    idx_t k = c - 1;                          /* 0‑based index for Stirling1 */

    /* Allocate arrays for signless Stirling numbers of the first kind
       up to row r‑1.  us1[i][j] holds S1(i,j).  */
    seq_t **us1 = (seq_t **)malloc(r * sizeof(seq_t *));
    for (idx_t i = 0; i < r; ++i) {
        us1[i] = (seq_t *)calloc(i + 1, sizeof(seq_t));
    }
    us1[0][0] = 1; /* S1(0,0) = 1 */
    for (idx_t i = 1; i < r; ++i) {
        us1[i][0] = 0;
        for (idx_t j = 1; j <= i; ++j) {
            if (j == i) {
                us1[i][j] = 1;
            } else {
                seq_t a_val = us1[i - 1][j - 1];
                seq_t b_val = ((seq_t)(i - 1)) * us1[i - 1][j];
                us1[i][j] = a_val + b_val;
            }
        }
    }

    /* Compute Stirling numbers of the second kind for row r.  */
    seq_t *prev = (seq_t *)calloc(r + 1, sizeof(seq_t)); /* S2(0,0) = 1 */
    seq_t *curr = (seq_t *)calloc(r + 1, sizeof(seq_t));
    prev[0] = 1;
    for (idx_t row = 1; row <= r; ++row) {
        curr[0] = 0;
        for (idx_t col = 1; col <= row; ++col) {
            seq_t a_val = prev[col - 1];
            seq_t b_val = ((seq_t)col) * prev[col];
            curr[col] = a_val + b_val;
        }
        /* swap prev and curr pointers for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Sum over i = 1..r of S2(r,i) * S1(i-1,k).  */
    seq_t result = 0;
    for (idx_t i = 1; i <= r; ++i) {
        seq_t s2_val = prev[i];
        seq_t s1_val = (k <= (i - 1)) ? us1[i - 1][k] : 0;
        result += s2_val * s1_val;
    }

    /* Free temporary memory.  */
    for (idx_t i = 0; i < r; ++i) {
        free(us1[i]);
    }
    free(us1);
    free(prev);
    free(curr);

    return result;
}
