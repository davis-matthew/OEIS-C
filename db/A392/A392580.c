/*
 * A392580  Expansion of 1 / ((1-x)^5 - x)^2.
 * https://oeis.org/A392580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 56*a(n-2) + 140*a(n-3) - 230*a(n-4) * +
 *            262*a(n-5) - 212*a(n-6) + 120*a(n-7) - 45*a(n-8) * + 10*a(n-9) -
 *            a(n-10) * Initial values: a(0)=1, a(1)=12, a(2)=88, a(3)=524,
 *            a(4)=2810, * a(5)=14198, a(6)=69068, a(7)=327240, a(8)=1520519,
 *            * a(9)=6959930
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_cap = 0;         /* allocated size of cache */
static idx_t cached_max = (idx_t)-1; /* largest n that has been filled */

/* Allocate initial cache with the first ten terms. */
static void init_cache(void) {
    if (cache != NULL) return;

    cache_cap = 10;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL) exit(1); /* hard failure */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)12;
    cache[2] = (seq_t)88;
    cache[3] = (seq_t)524;
    cache[4] = (seq_t)2810;
    cache[5] = (seq_t)14198;
    cache[6] = (seq_t)69068;
    cache[7] = (seq_t)327240;
    cache[8] = (seq_t)1520519;
    cache[9] = (seq_t)6959930;

    cached_max = 9;
}

/* Ensure the cache is large enough and contains all terms up to n. */
static void ensure_cache(idx_t n) {
    idx_t i;

    init_cache();

    /* Grow the array if needed. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;          /* simple growth factor */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) exit(1);               /* hard failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute terms beyond the current maximum. */
    for (i = cached_max + 1; i <= n; ++i) {
        /* Only compute when i >= 10; otherwise values already known. */
        seq_t val = 0;
        if (i >= 1) val += cache[i - 1] * (seq_t)12;
        if (i >= 2) val -= cache[i - 2] * (seq_t)56;
        if (i >= 3) val += cache[i - 3] * (seq_t)140;
        if (i >= 4) val -= cache[i - 4] * (seq_t)230;
        if (i >= 5) val += cache[i - 5] * (seq_t)262;
        if (i >= 6) val -= cache[i - 6] * (seq_t)212;
        if (i >= 7) val += cache[i - 7] * (seq_t)120;
        if (i >= 8) val -= cache[i - 8] * (seq_t)45;
        if (i >= 9) val += cache[i - 9] * (seq_t)10;
        if (i >= 10) val -= cache[i - 10] * (seq_t)1;

        cache[i] = val;
    }

    if ((idx_t)n > cached_max) cached_max = n;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
