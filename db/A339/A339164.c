/*
 * A339164  Number of compositions (ordered partitions) of n into distinct
 *          parts, the least being 3.
 * https://oeis.org/A339164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥0} (k+1)! * p_k(n‑3(k+1)) where p_k(m) is the number
 *            of partitions of m into exactly k distinct parts. The sum is
 *            taken over all k such that n ≥ (k+1)(k+6)/2
 * Range:     u8 n <= 24, u16 n <= 45, u32 n <= 95, u64 n <= 223,
 *            u128 n <= 570, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
 
seq_t a(idx_t n)
{
    /* Find the largest k for which a composition is possible. */
    int k_max = -1;
    for (int k = 0; ; ++k) {
        idx_t min_n = ((idx_t)(k + 1) * (k + 6)) / 2;
        if (min_n > n) break;
        k_max = k;
    }
    if (k_max < 0) return (seq_t)0;          /* n < 3 → no composition */
 
    /* The tables need to handle k up to k_max.  columns = k_max+1. */
    static idx_t maxN = 0;          /* largest n for which tables are built */
    static size_t cols = 0;          /* number of k-cols in the table */
    static seq_t *P = NULL;         /* DP table p_k(m) */
    static seq_t *fact = NULL;      /* factorial table */
 
    size_t new_cols = (size_t)(k_max + 1);
    if (n > maxN || new_cols > cols) {
        /* Allocate new tables large enough for this n and k_max. */
        size_t rows = (size_t)(n + 1);   /* row index = sum m ≤ n */
        seq_t *newP = (seq_t *)calloc(rows * new_cols, sizeof(seq_t));
        if (!newP) return (seq_t)0;     /* allocation failure */
        /* Base case: p_0(0) = 1 */
        for (size_t j = 0; j < new_cols; ++j)
            newP[0 * new_cols + j] = (j == 0) ? (seq_t)1 : (seq_t)0;
        /* Build DP table for k ≥ 1. */
        for (int k = 1; k <= k_max; ++k) {
            for (int m = k; m <= (int)n; ++m) {
                seq_t a1 = newP[(m - k) * new_cols + k];
                seq_t a2 = newP[(m - k) * new_cols + (k - 1)];
                newP[m * new_cols + k] = a1 + a2;
            }
        }
        /* Build factorial table up to (k_max + 1)! */
        seq_t *newFact = (seq_t *)calloc(new_cols + 1, sizeof(seq_t));
        if (!newFact) { free(newP); return (seq_t)0; }
        newFact[0] = (seq_t)1;
        for (int i = 1; i <= (int)k_max + 1; ++i)
            newFact[i] = newFact[i - 1] * (seq_t)i;
        /* Free old tables. */
        if (P) free(P);
        if (fact) free(fact);
        /* Store new tables. */
        P = newP;
        fact = newFact;
        maxN = n;
        cols = new_cols;
    }
    /* Now compute a(n). */
    seq_t total = (seq_t)0;
    for (int k = 0; k <= k_max; ++k) {
        idx_t min_n = ((idx_t)(k + 1) * (k + 6)) / 2;
        if (min_n > n) break;                      /* safety guard */
        idx_t M = n - 3 * (k + 1);                 /* total of the other parts */
        if (M < (idx_t)k * (k + 1) / 2) continue;   /* cannot form distinct parts */
        seq_t contrib = fact[k + 1] * P[M * cols + k];
        total += contrib;
    }
    return total;
}
