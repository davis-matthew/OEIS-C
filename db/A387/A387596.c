/*
 * A387596  Number of maximum matchings in the n-Cameron graph.
 * https://oeis.org/A387596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 5*a(n-3) + a(n-4) (n ≥ 5) Initial
 *            values: a(1) = 7, a(2) = 20, a(3) = 53, a(4) = 147
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* indices start at 1; cache[0] unused */
static size_t cache_cap = 0;  /* number of slots allocated (≥ needed index + 1) */
static idx_t  init_n   = 0;   /* largest n for which cache[n] is valid */

/* Ensure cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n) {
    if (!cache_cap || cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;            /* grow until > n    */
        /* For C++ builds where seq_t is not a POD, use a cast */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);                               /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Compute a(n).  Works for any order of calls. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;                                 /* undefined below 1 */

    /* Ensure we have enough space */
    ensure_capacity(n);

    /* Initialise seed values if not done yet */
    if (init_n < 4) {
        /* array indices 1..4 (index 0 unused) */
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)53;
        cache[4] = (seq_t)147;
        init_n = 4;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = init_n + 1; i <= n; ++i) {
        seq_t t = cache[i-1] * (seq_t)3
                + cache[i-2]
                - cache[i-3] * (seq_t)5
                + cache[i-4];
        cache[i] = t;
    }
    init_n = n;

    return cache[n];
}
