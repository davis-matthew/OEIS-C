/*
 * A267809  a(1)=a(2)=1; if n>2 then a(n) = a(n-2) + (a(n-1) mod 10).
 * https://oeis.org/A267809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=a(2)=1; a(n)=a(n-2)+(a(n-1) mod 10)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last   = 0;   /* largest index computed so far */

    /* Ensure we can index up to n (using 1‑based indexing) */
    size_t needed = (size_t)n + 1;            /* index 0 unused */
    if (needed > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();              /* out of memory */
        if (cache) {
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* Initialise the known terms */
    if (last < 2) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        last = 2;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t mod10 = cache[i - 1] % (seq_t)10;
        cache[i]     = cache[i - 2] + mod10;
    }
    last = n;

    return cache[n];
}
