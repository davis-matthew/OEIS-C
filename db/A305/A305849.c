/*
 * A305849  Positions of 2 in the difference sequence of A305847.
 * https://oeis.org/A305849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A305849:
   a(1) = 3
   a(n) = 3*n - count{ a(k) ≤ n-1 , 1 ≤ k < n }   for n > 1   (1‑based indices)
   This recurrence follows directly from the definition of the sequence. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static array holding computed values, 1‑based indexing. */
    static seq_t *seq = NULL;
    static idx_t  cap   = 0;   /* allocated capacity (excluding index 0) */
    static idx_t  last  = 0;   /* largest computed index */
    static idx_t  j     = 0;   /* number of terms <= current i-1 */

    /* Grow the array if needed. Allocate one more slot for 1‑based indexing. */
    if (n > cap) {
        size_t new_cap = cap ? (size_t)cap << 1 : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq = (seq_t *)realloc(seq, sizeof(seq_t) * (new_cap + 1));
        cap = (idx_t)new_cap;
    }

    /* Compute terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Update j: number of already computed terms that are <= i-1.
           Since seq is increasing, j can only grow. */
        while (j + 1 <= last && seq[j + 1] <= i - 1)
            ++j;

        /* Apply the recurrence. */
        if (i == 1) {
            /* a(1) = 3 as per initial condition. */
            seq[i] = (seq_t)3;
        } else {
            seq[i] = ((seq_t)3 * i) - (seq_t)j;
        }
        last = i;
    }
    return seq[n];
}
