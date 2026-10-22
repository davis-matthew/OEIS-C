/*
 * A385831  a(0) = 1; a(n) = Sum_{k=0..n-1} (1 + k^3) * a(k) * a(n-1-k).
 * https://oeis.org/A385831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1; a(n) = Sum_{k=0}^{n-1} (1 + k^3) * a(k) * a(n-1-k) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy static cache: a_cache[i] holds a(i) for i <= computed */
    static seq_t *a_cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;  /* largest index computed so far */

    /* Expand cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed; return 0 (undefined, but guarantees no crash) */
            return (seq_t)0;
        }
        a_cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize a(0) if not yet done */
    if (computed == 0 && a_cache[0] == 0) {
        a_cache[0] = (seq_t)1;
        computed = 0;
    }

    /* Compute up to index n if needed */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t k_seq = (seq_t)k;
            seq_t wk = (seq_t)1 + k_seq * k_seq * k_seq;          /* 1 + k^3 */
            seq_t prod = a_cache[k] * a_cache[i - 1 - k];
            sum += wk * prod;
        }
        a_cache[i] = sum;
    }
    computed = n;
    return a_cache[n];
}
