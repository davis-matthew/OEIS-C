/*
 * A264251  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 0,1 0,-1 0,2 1,0 or
 *          -1,0.
 * https://oeis.org/A264251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A264251 – OGF: x*(1-x)*(10+33x+20x^2-18x^3-12x^4+3x^5+x^6)
/* Recurrence (empirical): 
 *   a(n) = a(n-1) + 16 a(n-2) + 9 a(n-3) - 29 a(n-4) 
 *          - 9 a(n-5) + 16 a(n-6) - a(n-7) - a(n-8)
 * Initial values (offset 1):
 *   a(1)=10, a(2)=33, a(3)=180, a(4)=760,
 *   a(5)=3653, a(6)=16401, a(7)=76330, a(8)=348480
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/free_seq.h"

#ifdef __cplusplus
/* When compiled as C++ use new/delete which invoke constructors */
static seq_t *alloc_seq(size_t cap)      { return new seq_t[cap]; }

static seq_t *realloc_seq(seq_t *old, size_t old_cap, size_t new_cap) {
    seq_t *newp = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i) newp[i] = old[i];
    delete[] old;
    return newp;
}
#else
/* Plain C – use malloc/free, seq_t is assumed to be a POD type */
static seq_t *alloc_seq(size_t cap)      { return (seq_t*)malloc(cap * sizeof(seq_t)); }

static seq_t *realloc_seq(seq_t *old, size_t old_cap, size_t new_cap) {
    seq_t *newp = (seq_t*)realloc(old, new_cap * sizeof(seq_t));
    /* realloc keeps existing values; no copying needed */
    return newp;
}
#endif

seq_t a(idx_t n)
{
    /* Index 0 is unused – the sequence starts at a(1) */
    if (n == 0) return (seq_t)0;

    /* Static state for memoisation (not thread‑safe) */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t  capacity  = 0;      /* current allocated size */
    static idx_t   max_cached = 0;     /* largest n for which cache[n] is valid */

    /* Initialise storage on first call */
    if (cache == NULL) {
        capacity   = 8;                 /* start with space for indices 0..8 */
        cache      = alloc_seq(capacity + 1); /* +1 because we use index n */
        if (cache == NULL) return (seq_t)0;   /* allocation failure */

        /* Base terms – a(0) = 0 is unused but convenient for indexing */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)33;
        cache[3] = (seq_t)180;
        cache[4] = (seq_t)760;
        cache[5] = (seq_t)3653;
        cache[6] = (seq_t)16401;
        cache[7] = (seq_t)76330;
        cache[8] = (seq_t)348480;
        max_cached = 8;
    }

    /* Grow the array if the requested index is too large */
    if ((size_t)n > capacity) {
        size_t new_cap = capacity;
        while (new_cap < (size_t)n) new_cap <<= 1;   /* double until sufficient */
        seq_t *new_cache = realloc_seq(cache, capacity + 1, new_cap + 1);
        if (new_cache == NULL) return (seq_t)0; /* allocation failure */
        cache     = new_cache;
        capacity  = new_cap;
        /* Newly allocated elements are default‑constructed (zero) */
    }

    /* If the term is already known, return it directly */
    if (n <= max_cached) return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* Use unsigned arithmetic on seq_t; intermediate results are sequence
           terms themselves, which grow exactly as required for the recurrence. */
        seq_t term = cache[i-1];
        term += (seq_t)16 * cache[i-2];
        term += (seq_t)9  * cache[i-3];
        term -= (seq_t)29 * cache[i-4];
        term -= (seq_t)9  * cache[i-5];
        term += (seq_t)16 * cache[i-6];
        term -= cache[i-7];
        term -= cache[i-8];
        cache[i] = term;
    }
    max_cached = n;
    return cache[n];
}
