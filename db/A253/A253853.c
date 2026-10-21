/*
 * A253853  a(n) = 1 + a(n-2)*a(n-3), with a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A253853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(n-2)*a(n-3) Initial values: a(0)=a(1)=a(2)=1.
 *            Computes the sequence iteratively with memoisation
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 15, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t len   = 0;

static void grow_cache(size_t min_cap) {
    if (cap >= min_cap) return;
    size_t new_cap = (cap == 0) ? 4 : cap << 1;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);          /* abort on allocation failure */
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n) {
    if (n < 0) return 0; /* undefined, but safe guard */

    /* initialise cache on first call */
    if (!cache) {
        grow_cache(4);
        cache[0] = 1; cache[1] = 1; cache[2] = 1;
        len = 3;
    }

    if ((idx_t)len > n) return cache[n];

    grow_cache((size_t)n + 1);      /* ensure enough room for n */

    for (idx_t i = (idx_t)len; i <= n; ++i) {
        seq_t prod = cache[i-2] * cache[i-3];
        cache[i] = 1 + prod;
    }
    len = n + 1;
    return cache[n];
}
