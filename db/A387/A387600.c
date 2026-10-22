/*
 * A387600  a(n) = (1/2) * Sum_{k=0..floor(n/3)} 2^k *
 *          binomial(2*n-4*k+2,2*k+1).
 * https://oeis.org/A387600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3) + 4*a(n-4) - 4*a(n-6)
 *            Initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=8, a(4)=25, a(5)=62
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;        /* number of stored terms */

/* Base values for a(0) .. a(5) */
static const seq_t base[6] = {1, 2, 3, 8, 25, 62};

/* Ensure that cache contains values up to index n */
static void ensure_cache(idx_t n) {
    if (n < cache_size) return;

    /* Allocate new array of required size */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
    if (!new_cache) return; /* allocation failed – in this environment
                              it is assumed that enough memory is available */

    /* Copy old values to new array */
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];

    /* Compute new entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 6) {
            new_cache[i] = base[i];
        } else {
            /* Apply the recurrence */
            seq_t t1 = new_cache[i - 1];
            seq_t t2 = new_cache[i - 3];
            seq_t t3 = new_cache[i - 4];
            seq_t t4 = new_cache[i - 2];
            seq_t t5 = new_cache[i - 6];

            seq_t pos = t1 * (seq_t)2 + t2 * (seq_t)4 + t3 * (seq_t)4;
            seq_t neg = t4 + t5 * (seq_t)4;

            new_cache[i] = pos - neg;
        }
    }

    /* Replace old cache */
    if (cache) free(cache);
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
