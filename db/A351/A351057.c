/*
 * A351057  G.f. A(x) satisfies: A(x) = 1 + x + x^2 * A(x/(1 - 6*x)) / (1 -
 *          6*x).
 * https://oeis.org/A351057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(1) = 1; a(n) = Σ_{k=0}^{n-2} C(n-2,k) * 6^k *
 *            a(n-k-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for previously computed terms.
   a_cache[i] holds a(i). */
static seq_t *a_cache = NULL;      /* pointer to cache array */
static size_t a_cache_cap = 0;     /* allocated capacity */
static size_t a_cache_size = 0;    /* number of elements computed so far */

/* Ensure the cache contains values up to index n (inclusive). */
static void ensure_a_cache(idx_t n)
{
    if ((size_t)n + 1 <= a_cache_size)
        return;                       /* already available */

    /* Grow capacity (exponential growth). */
    size_t new_cap = a_cache_cap ? a_cache_cap : 1;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;

    seq_t *new_ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_ptr)                     /* allocation failed */
        exit(1);
    a_cache = new_ptr;
    a_cache_cap = new_cap;

    /* Compute missing values. */
    for (idx_t i = a_cache_size; i <= n; ++i) {
        if (i == 0 || i == 1) {
            a_cache[i] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        idx_t m = i - 2;              /* length of the inner sum */
        seq_t binom = (seq_t)1;       /* C(m,0) */
        seq_t pow6  = (seq_t)1;       /* 6^0 */

        for (idx_t k = 0; k <= m; ++k) {
            if (k > 0) {              /* update binomial and power of 6 */
                binom = binom * (seq_t)(m - k + 1) / (seq_t)k;
                pow6  *= (seq_t)6;
            }
            seq_t term = binom * pow6 * a_cache[i - k - 2];
            sum += term;
        }
        a_cache[i] = sum;
    }
    a_cache_size = (size_t)n + 1;
}

/* Public function required by the task. */
seq_t a(idx_t n)
{
    ensure_a_cache(n);
    return a_cache[(size_t)n];
}
