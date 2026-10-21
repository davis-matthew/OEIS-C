/*
 * A243600  Number of length n+2 0..3 arrays with no three elements in a row
 *          with pattern aba (with a!=b) and new values 0..3 introduced in
 *          0..3 order.
 * https://oeis.org/A243600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) - 3*a(n-3) + 3*a(n-4) + a(n-5),
 *            a(1)=4, a(2)=10, a(3)=28, a(4)=85, a(5)=269
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based cache */
static idx_t  cache_size = 0;        /* largest index computed so far  */
static size_t cache_cap  = 0;        /* allocated array size (>= cache_size+1) */

static const seq_t init_vals[5] = {
    (seq_t)4,
    (seq_t)10,
    (seq_t)28,
    (seq_t)85,
    (seq_t)269
};

/* Ensure that values up to index n are computed and stored in cache. */
static void ensure_cache(idx_t n) {
    if (n <= cache_size) return;               /* already computed */

    /* Compute enough capacity. We always keep at least 5 elements. */
    size_t need = (size_t)n + 1;                /* include index 0 unused */
    if (need < 6) need = 6;

    if (need > cache_cap) {
        seq_t *np = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!np) exit(EXIT_FAILURE);
        cache = np;
        if (cache_cap == 0) {
            for (idx_t i = 1; i <= 5 && i < need; ++i)
                cache[i] = init_vals[i-1];
        }
        cache_cap = need;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 5) continue;   /* initial values already set */
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        seq_t a5 = cache[i-5];
        cache[i] = a1 * 5 - a2 * 5 - a3 * 3 + a4 * 3 + a5;
    }

    cache_size = n;
}

/* Public function required by the interface. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;               /* undefined below offset */
    ensure_cache(n);
    return cache[n];
}
