/*
 * A333554  a(0) = 9; thereafter a(n+1) = 3*a(n)^4 + 4*a(n)^3 for n >= 1.
 * https://oeis.org/A333554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=9, a(n+1)=3*a(n)^4+4*a(n)^3 */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_max = 0;
static size_t cache_cap = 0;

/* Initialise the cache with a(0)=9 */
static void init_cache(void) {
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = 1;
        cache[0] = (seq_t)9;
        cache_max = 0;
    }
}

/* Grow the cache and compute terms up to n */
static void ensure_cache(idx_t n) {
    if (n <= cache_max) return;

    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t a   = cache[i-1];
        seq_t a2  = a * a;
        seq_t a3  = a2 * a;
        seq_t a4  = a3 * a;
        seq_t term = ((seq_t)3) * a4 + ((seq_t)4) * a3;
        cache[i] = term;
    }
    cache_max = n;
}

/* Return a(n) */
seq_t a(idx_t n) {
    init_cache();
    if (n == 0) return cache[0];
    ensure_cache(n);
    return cache[n];
}
