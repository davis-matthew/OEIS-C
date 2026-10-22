/*
 * A300367  Number of nX3 0..1 arrays with every element equal to 2 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + 5*a(n-4) + 6*a(n-5) + 4*a(n-6) +
 *            6*a(n-7) + a(n-8) - a(n-12) + 2*a(n-13) - a(n-14) Initial values
 *            (a(1)…a(14)): 0,1,0,3,3,8,16,32,70,135,293,585,1221,2496
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* dynamic array of computed values */
static size_t cache_cap = 0;    /* allocated capacity */
static idx_t max_computed  = 0; /* highest n that has been computed (1‑based) */

static const seq_t initVals[14] = {
    (seq_t)0,   (seq_t)1,   (seq_t)0,   (seq_t)3,   (seq_t)3,
    (seq_t)8,   (seq_t)16,  (seq_t)32,  (seq_t)70,  (seq_t)135,
    (seq_t)293, (seq_t)585, (seq_t)1221,(seq_t)2496
};

/* Ensure cache contains a(1)…a(n) */
static void ensure_cache(idx_t n)
{
    if (n <= max_computed) return;               /* nothing to do */

    /* resize if necessary */
    if (cache == NULL || n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        cache   = tmp;
        cache_cap = (size_t)n;
    }

    /* initialise from the list of known terms */
    if (max_computed < 14) {
        idx_t upto = (n < 14 ? n : 14);
        for (idx_t i = max_computed + 1; i <= upto; ++i)
            cache[i-1] = initVals[i-1];
        max_computed = upto;
    }

    /* compute the remaining terms by the empirical recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 14) {                      /* should already be set */
            cache[i-1] = initVals[i-1];
            continue;
        }
        seq_t val =
            cache[i-3]          /* a(i-2) */
          + 2*cache[i-4]        /* 2*a(i-3) */
          + 5*cache[i-5]        /* 5*a(i-4) */
          + 6*cache[i-6]        /* 6*a(i-5) */
          + 4*cache[i-7]        /* 4*a(i-6) */
          + 6*cache[i-8]        /* 6*a(i-7) */
          + cache[i-9]          /* a(i-8) */
          - cache[i-13]         /* -a(i-12) */
          + 2*cache[i-14]       /* +2*a(i-13) */
          - cache[i-15];        /* -a(i-14) */
        cache[i-1] = val;
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* sequence is defined from offset 1 */
    ensure_cache(n);
    return cache[n-1];
}
