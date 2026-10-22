/*
 * A362156  Expansion of -1/(1 - x * sqrt(1-4*x)).
 * https://oeis.org/A362156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Expansion of -1/(1 - x * sqrt(1-4*x))
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 18, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via convolution: a_0 = -1, a_n = a_{n-1} - 2 * Σ_{k=0}^{n-2} C_k * a_{n-k-2}
   where C_k are Catalan numbers (C_0 = 1, C_{i+1} = C_i * (4*i+2)/(i+2)). */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *a_cache = NULL;          /* cached a values */
    static size_t a_cap = 0;              /* number of a values allocated */
    static seq_t *c_cache = NULL;         /* cached Catalan numbers C_0 .. C_{n-2} */
    static size_t c_cap = 0;              /* number of Catalan entries allocated */

    /* Ensure a[0] is defined */
    if (a_cap == 0) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) return 0;          /* allocation failure */
        a_cache[0] = -1;
        a_cap = 1;
    }

    /* Expand cache if necessary to compute up to n */
    if ((idx_t)n >= (idx_t)a_cap) {
        /* Resize a_cache to hold n+1 elements */
        size_t new_a_cap = (size_t)(n + 1);
        if (new_a_cap < 2) new_a_cap = 2;
        seq_t *tmp_a = (seq_t *)realloc(a_cache, new_a_cap * sizeof(seq_t));
        if (!tmp_a) return 0;            /* allocation failure */
        a_cache = tmp_a;
        /* Prepare Catalan numbers up to index n-2 (needs C_0 .. C_{n-2}) */
        if ((size_t)n >= 2) {
            size_t needed_c_cap = (size_t)(n - 1);  /* number of Catalan entries needed */
            if (needed_c_cap > c_cap) {
                seq_t *tmp_c = (seq_t *)realloc(c_cache, needed_c_cap * sizeof(seq_t));
                if (!tmp_c) return 0;
                c_cache = tmp_c;
                /* Compute missing Catalan numbers */
                if (c_cap == 0) {
                    /* Initialize C_0 */
                    c_cache[0] = 1;
                    c_cap = 1;
                }
                for (idx_t i = (idx_t)c_cap; i <= (idx_t)needed_c_cap - 1; ++i) {
                    /* C_i = C_{i-1} * (4*(i-1)+2) / (i+1) */
                    seq_t prev = c_cache[i - 1];
                    seq_t num  = prev * ((seq_t)(4 * (i - 1) + 2));
                    seq_t den  = (seq_t)(i + 1);
                    c_cache[i] = num / den;
                }
                c_cap = needed_c_cap;
            }
        }

        /* Compute a values from current size up to n */
        for (idx_t i = (idx_t)a_cap; i <= n; ++i) {
            seq_t sum = a_cache[i - 1];          /* term from s_0 = 1 */
            if (i >= 2) {
                for (idx_t k = 0; k <= i - 2; ++k) {
                    seq_t prod = c_cache[k];
                    prod *= a_cache[i - k - 2];
                    prod *= (seq_t)2;
                    sum -= prod;
                }
            }
            a_cache[i] = sum;
        }
        a_cap = new_a_cap;
    }

    return a_cache[n];
}
