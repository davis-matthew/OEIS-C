/*
 * A258973  The number of plain lambda terms presented by de Bruijn indices,
 *          see Bendkowski et al., where zeros have no weight.
 * https://oeis.org/A258973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = 1 + a(n-1) + Σ_{i=0}^{n-1} a(i)*a(n-1-i) (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequenced integer type seq_t and index type idx_t are provided by the
   build environment. The implementation uses memoisation so that the
   function works correctly irrespective of the order and multiplicity
   of calls.  */

#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A258973. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation, accessible across calls. */
    static seq_t *cache = NULL;     /* array holding a(k), k ≤ current cache size-1 */
    static idx_t cache_size = 0;    /* number of entries currently allocated */

    /* --- Ensure the cache is large enough for index n --- */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {                 /* allocation failure – terminate */
            abort();
        }
        cache = tmp;
        /* Initialise newly allocated entries to zero (uncomputed). */
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = (seq_t)0;
        }
        cache_size = new_size;
    }

    /* --- Initialise the first term (a(0) = 1) if not already done --- */
    if (cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;
    }

    /* --- Compute any missing terms up to n --- */
    for (idx_t k = 1; k <= n; ++k) {
        if (cache[k] != (seq_t)0) {
            continue;            /* already computed */
        }

        /* Compute Σ_{i=0}^{k-1} a(i)*a(k-1-i) using symmetry about the centre. */
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i <= (k - 1) / 2; ++i) {
            idx_t j = (k - 1) - i;
            seq_t prod = cache[i] * cache[j];
            if (i < j) {
                sum += prod;       /* two symmetric terms */
                sum += prod;
            } else {
                sum += prod;       /* centre term when i == j */
            }
        }

        /* Apply the recurrence: a(k) = 1 + a(k-1) + sum. */
        cache[k] = (seq_t)1 + cache[k - 1] + sum;
    }

    return cache[n];
}
