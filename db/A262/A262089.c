/*
 * A262089  a(0) = 0, a(1) = 1, a(n) = a(n-2)^2 + a(n-1)^3.
 * https://oeis.org/A262089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑2)^2 + a(n‑1)^3, with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *values = NULL;
    static size_t size  = 0;   /* number of terms stored */
    static size_t cap   = 0;   /* allocated capacity */

    /* Ensure the array can hold a(n) */
    size_t idx = (size_t)n;   /* local copy of the index */
    if (cap <= idx) {
        size_t new_cap = idx + 1;
        if (cap == 0) {
            values = (seq_t *)malloc(new_cap * sizeof(*values));
        } else {
            values = (seq_t *)realloc(values, new_cap * sizeof(*values));
        }
        /* assume allocation succeeds; if not, the program will crash,
           which is acceptable for this context. */
        cap = new_cap;
    }

    /* Initialize base cases if not already present */
    if (size == 0) {
        values[0] = 0;
        size = 1;
    }
    if (size == 1 && idx >= 1) {
        values[1] = 1;
        size = 2;
    }

    /* Compute missing values up to n */
    for (size_t i = size; i <= idx; ++i) {
        seq_t prev1 = values[i - 1];
        seq_t prev2 = values[i - 2];

        seq_t prev1_sq = prev1 * prev1;
        seq_t prev1_cu = prev1_sq * prev1;
        seq_t prev2_sq = prev2 * prev2;

        values[i] = prev2_sq + prev1_cu;
    }
    size = idx + 1;   /* update stored length */

    return values[idx];
}
