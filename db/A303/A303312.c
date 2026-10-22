/*
 * A303312  Number of nX6 0..1 arrays with every element equal to 0, 1, 4, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) is OEIS A303312.
   Initial values (n=1..12):
     13, 13, 67, 128, 410, 1221, 3453, 10223,
     30247, 88071, 257997, 758575.
   For n>12 the recurrence holds:
     a(n) = a(n-1)+2*a(n-2)+10*a(n-3)+6*a(n-4)
            -5*a(n-5)-15*a(n-6)-8*a(n-7)-4*a(n-8). */

#include <stdlib.h>  /* malloc, realloc, free */
#include <stdint.h>  /* uint64_t */

/* The two integer types are supplied by the build. */
extern seq_t _zero; /* dummy to avoid unused warning */

static const seq_t init[13] = {
    0,          /* index 0 unused */
    13, 13, 67, 128, 410, 1221, 3453, 10223,
    30247, 88071, 257997, 758575
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size (in terms of indices) */
static size_t cache_len = 0;   /* highest computed index */

/* Ensure the cache holds at least indices up to n. */
static void ensure_cache(idx_t n) {
    if (cache == NULL) {
        /* Allocate room for the initial 12 terms (index 0..12). */
        size_t cap = 13;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i)
            cache[i] = init[i];
        cache_cap = cap;
        cache_len = 12;
    }
    if ((size_t)n <= cache_len)
        return;
    /* Extend capacity if needed. */
    size_t needed = (size_t)n + 1;          /* indices 0..n incl. */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        /* Positive part: a(i-1)+2*a(i-2)+10*a(i-3)+6*a(i-4) */
        seq_t pos = cache[i - 1] + 2 * cache[i - 2]
                + 10 * cache[i - 3] + 6 * cache[i - 4];
        /* Negative part: 5*a(i-5)+15*a(i-6)+8*a(i-7)+4*a(i-8) */
        seq_t neg = 5 * cache[i - 5] + 15 * cache[i - 6]
                + 8 * cache[i - 7] + 4 * cache[i - 8];
        cache[i] = pos - neg;  /* pos >= neg for all n>=12 */
    }
    cache_len = (size_t)n;
}

seq_t a(idx_t n) {
    if (n <= 0)  /* offset starts at 1, but unused area treated as 0 */
        return 0;
    if ((size_t)n <= 12)            /* directly return the initial values */
        return init[(size_t)n];
    ensure_cache(n);
    return cache[(size_t)n];
}
