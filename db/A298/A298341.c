/*
 * A298341  a(n) = a(n-1) + a(n-2) + a([n/3]), where a(0) = 1, a(1) = 2, a(2)
 *          = 3.
 * https://oeis.org/A298341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(floor(n/3)), a(0)=1, a(1)=2, a(2)=3
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of a(i) values */
static idx_t cache_len = 0;          /* number of valid entries in cache   */
static idx_t cache_cap = 0;          /* allocated capacity of the array    */

/* Ensure cache has space for index `need_index` (inclusive). */
static void ensure_capacity(idx_t need_index) {
    if (cache_cap <= need_index) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= need_index) new_cap <<= 1;   /* double until enough */
        seq_t *new_arr = (seq_t *)realloc(cache,
                                          (size_t)new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);                       /* fatal error */
        cache     = new_arr;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* Initialize base cases only once */
    if (cache_len == 0) {
        ensure_capacity(2);                  /* indices 0,1,2 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache_len = 3;
    }

    if (n < cache_len)
        return cache[n];

    /* Compute a(i) for i = cache_len .. n */
    ensure_capacity(n);
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i/3];
    }
    cache_len = n + 1;                       /* update highest computed index */
    return cache[n];
}
