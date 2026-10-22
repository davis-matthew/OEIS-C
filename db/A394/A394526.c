/*
 * A394526  a(n) = n*(5^n - 1) + 1.
 * https://oeis.org/A394526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5^n - 1) + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Computes a(n) = n*(5^n - 1) + 1 with caching for efficiency.  */
seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *pow5_cache = NULL;   /* 5^i */
    static size_t pow5_cap   = 0;      /* allocated size */
    static size_t pow5_size  = 0;      /* number of values computed   */

    static seq_t *a_cache = NULL;      /* a(i) */
    static size_t a_cap   = 0;
    static size_t a_size  = 0;

    /* Ensure pow5_cache is large enough to contain 5^n */
    if ((size_t)n >= pow5_size) {
        size_t needed = (size_t)n + 1;
        if (needed > pow5_cap) {
            size_t new_cap = pow5_cap ? pow5_cap * 2 : 1;
            while (new_cap < needed) new_cap *= 2;
            pow5_cache = (seq_t*)realloc(pow5_cache, new_cap * sizeof(seq_t));
            pow5_cap = new_cap;
        }
        /* Compute missing 5^i values and corresponding a(i) values */
        for (size_t i = pow5_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                pow5_cache[0] = 1;
            } else {
                pow5_cache[i] = pow5_cache[i - 1] * 5;
            }
            /* Ensure a_cache is large enough for a(i) */
            if (i >= a_cap) {
                size_t new_cap = a_cap ? a_cap * 2 : 1;
                while (new_cap <= i) new_cap *= 2;
                a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
                a_cap = new_cap;
            }
            seq_t term = ((seq_t)i) * (pow5_cache[i] - 1) + 1;
            a_cache[i] = term;
        }
        pow5_size = (size_t)n + 1;
        a_size = pow5_size;
    }

    return a_cache[(size_t)n];
}
