/*
 * A319013  a(n) is the sum over each permutation of S_n of the least element
 *          of the descent set.
 * https://oeis.org/A319013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = (n+1)*a(n) + n², a(1) = 0 (valid for all n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A319013. */
seq_t a(idx_t n)
{
    /* static cache: cache[0] unused; cache[1] = 0; */
    static seq_t *cache   = NULL;
    static idx_t  cache_size = 0;   /* largest n currently cached */

    /* Ensure cache has room for indices up to n. */
    if (n > cache_size) {
        size_t new_cap = (size_t)(n + 1);          /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                        /* out of memory */
        cache = tmp;

        /* Initialise new entries. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = 0;                     /* a(1) = 0 */
            } else {
                /* a(i) = i * a(i-1) + (i-1)^2 */
                seq_t prev   = cache[i - 1];
                seq_t term   = (seq_t)(i - 1) * (seq_t)(i - 1); /* (i-1)^2 */
                seq_t prod   = (seq_t)i * prev;                 /* i * a(i-1) */
                cache[i] = prod + term;
            }
        }
        cache_size = n;
    }

    return cache[n];
}
