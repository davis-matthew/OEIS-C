/*
 * A279890  Expansion of x*(1 - x + 2*x^3 - x^4)/((1 - x)*(1 + x)*(1 - x +
 *          x^2)*(1 - x - x^2)).
 * https://oeis.org/A279890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - a(n-6) with initial
 *            values a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=7
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation using memoised linear recurrence. Any order of calls is supported. */
#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array holding computed terms   */
static size_t cap   = 0;             /* allocated size of cache               */
static size_t len   = 0;             /* number of terms that are initialized */

/* initialise cache with the first six terms */
static void init_cache(void) {
    if (cache) return;
    cap = 6;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) exit(1);            /* abort if allocation fails          */
    cache[0] = 0;
    cache[1] = 1;
    cache[2] = 1;
    cache[3] = 2;
    cache[4] = 4;
    cache[5] = 7;
    len = 6;
}

/* ensure that the cache is large enough to hold terms up to index n */
static void ensure_cap(idx_t n) {
    if (n + 1 <= cap) return;
    size_t new_cap = cap;
    while (new_cap <= (size_t)n + 1) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cap = new_cap;
}

seq_t a(idx_t n) {
    init_cache();                    /* first call initialises cache */
    if (n < len) return cache[n];
    ensure_cap(n);                   /* grow array if necessary */
    for (idx_t i = len; i <= n; ++i) {
        seq_t term1 = cache[i-1];                     /* 2*a(i-1) */
        term1 = term1 + term1;
        seq_t term2 = cache[i-3];                     /* 2*a(i-3) */
        term2 = term2 + term2;
        seq_t term3 = cache[i-4];                     /* 2*a(i-4) */
        term3 = term3 + term3;
        seq_t term4 = cache[i-6];                     /* a(i-6)  */
        seq_t val = term1 + term3;                    /* 2*a(i-1)+2*a(i-4) */
        val = val - term2;                            /* minus 2*a(i-3)   */
        val = val - term4;                            /* minus a(i-6)     */
        cache[i] = val;
    }
    len = (size_t)n + 1;
    return cache[n];
}
