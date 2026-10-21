/*
 * A256911  Number of terms in the enhanced triangular-number representation
 *          of n.
 * https://oeis.org/A256911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 394, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enhanced triangular-number representation:
   a(n) = number of distinct triangular numbers used in the greedy decomposition
   of n into triangular numbers.  The decomposition repeatedly chooses the
   largest triangular number ≤ the current remainder (allowing repeats).
   The count is the number of distinct triangular values that appear. */

#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n).  The function is self‑contained and does not use any external
   utilities.  It allocates a small amount of memory per call and frees it
   before returning. */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 by definition. */
    if (n == 0) return 1;

    seq_t N = (seq_t)n;              /* promote index to value type */

    /* Build the triangular numbers T_1, T_2, … ≤ N. */
    size_t tri_cnt = 0;
    for (idx_t k = 1;; ++k) {
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;
        if (t > N) break;
        ++tri_cnt;
    }

    /* Allocate arrays to hold the triangular numbers and flags for used ones. */
    seq_t *tri = (seq_t *)malloc(tri_cnt * sizeof(seq_t));
    bool  *used = (bool  *)calloc(tri_cnt, sizeof(bool));
    if (!tri || !used) {
        /* Memory allocation failed – return 0 (unlikely for small n). */
        free(tri);
        free(used);
        return 0;
    }

    /* Fill the triangular number array. */
    idx_t k = 1;
    for (size_t i = 0; i < tri_cnt; ++i, ++k) {
        tri[i] = (seq_t)k * ((seq_t)k + 1) / 2;
    }

    seq_t rem = N;
    size_t distinct = 0;

    /* Greedy decomposition, counting distinct triangular numbers. */
    while (rem > 0) {
        /* Find largest triangular number ≤ rem. */
        size_t i = tri_cnt;
        while (i > 0 && tri[--i] > rem) { /* move down until suitable */ }

        rem -= tri[i];
        if (!used[i]) {
            used[i] = true;
            ++distinct;
        }
    }

    free(tri);
    free(used);
    return (seq_t)distinct;
}
