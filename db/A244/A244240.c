/*
 * A244240  Number of partitions of n into 4 parts such that every i-th
 *          smallest part (counted with multiplicity) is different from i.
 * https://oeis.org/A244240
 *
 * Offset:    14    (a(14) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-5) + a(n-8) + a(n-9) - a(n-10)
 *            for n > 26, with initial values: a(14)=1, a(15)=4, a(16)=7,
 *            a(17)=11, a(18)=15, a(19)=19, a(20)=25, a(21)=30, a(22)=37,
 *            a(23)=44, a(24)=53, a(25)=61, a(26)=72. For n < 14 the function
 *            returns 0
 * Range:     u8 n <= 37, u16 n <= 214, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 14) return 0;          /* behaviour for n < 14 is irrelevant */

    /* Persistent cache of previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* allocated size (indices 0 … cache_size-1) */
    static idx_t last_computed = 26; /* highest index having a stored value */

    /* ensure the cache is large enough for the requested n */
    idx_t required = n;
    if (required < last_computed) required = last_computed;
    if (cache_size <= required) {
        idx_t new_size = required + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure: safe default */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* initialise the known first values (only once) */
    if (last_computed == 26 && cache[14] == (seq_t)0) {
        static const seq_t base[13] = {
            (seq_t)1, (seq_t)4, (seq_t)7, (seq_t)11, (seq_t)15,
            (seq_t)19, (seq_t)25, (seq_t)30, (seq_t)37, (seq_t)44,
            (seq_t)53, (seq_t)61, (seq_t)72
        };
        for (idx_t i = 0; i < 13; ++i) cache[14 + i] = base[i];
    }

    /* if the requested index is already computed, simply return it */
    if (n <= last_computed) return cache[n];

    /* compute values up to n using the recurrence */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t term = cache[k-1] + cache[k-2] + cache[k-8] + cache[k-9];
        seq_t twice = cache[k-5];
        twice += twice;                /* 2 * a(k-5) */
        term -= twice;
        term -= cache[k-10];
        cache[k] = term;
    }
    last_computed = n;
    return cache[n];
}
