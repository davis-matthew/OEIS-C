/*
 * A397679  a(n) is the number of tilings of a 8 X 5n rectangle into
 *          I-pentominoes.
 * https://oeis.org/A397679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) a(0)
 *            = 1, a(1) = 5, a(2) = 41, a(3) = 325, a(4) = 2561
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache the computed values of the sequence */
static seq_t *cache      = NULL;   /* array of computed terms */
static size_t cache_cap  = 0;      /* allocated capacity of the array */
static idx_t  computed_max = (idx_t)(-1); /* highest index already computed */

/* Initialize the cache with the first five terms. */
static void init_cache(void) {
    if (cache != NULL) return;          /* already initialized */
    const seq_t init[5] = {1, 5, 41, 325, 2561};
    cache_cap = 10;                     /* initial capacity */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();                /* failure is fatal for the build */
    for (idx_t i = 0; i < 5; ++i) cache[i] = init[i];
    computed_max = 4;                   /* we have terms up to a(4) */
}

/* Ensure the cache is large enough to hold indices up to n. */
static void ensure_capacity(idx_t n) {
    if ((idx_t)cache_cap > n) return;
    size_t new_cap = cache_cap;
    while ((idx_t)new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache  = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n) {
    init_cache();                       /* make sure the cache exists */

    if (n <= computed_max) {
        return cache[n];                /* already computed */
    }

    ensure_capacity(n);                 /* guarantee space for new terms */

    /* Compute new terms one by one. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t t = 9 * cache[i - 1];
        t -= 10 * cache[i - 2];
        t += 10 * cache[i - 3];
        t -= 5  * cache[i - 4];
        t +=    cache[i - 5];
        cache[i] = t;
    }
    computed_max = n;
    return cache[n];
}
