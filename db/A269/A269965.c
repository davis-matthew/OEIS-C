/*
 * A269965  Start with a square; at each stage add a square at each expandable
 *          vertex so that the ratio of the side of the squares at stage n+1
 *          and at stage n is the golden ratio phi=0.618...; a(n) is the
 *          number of squares in a portion of the n-th stage (see below).
 * https://oeis.org/A269965
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n >= 8, a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) + 2*a(n-4) +
 *            2*a(n-5) + 5 with initial values a(1)=a(2)=a(3)=a(4)=0, a(5)=1,
 *            a(6)=3, a(7)=10. For n < 5 the value is defined as 0
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 30, u64 n <= 57, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>


/* ------------------------------------------------------------------ */
/* Helper: grow a dynamic array of seq_t                              */
/* ------------------------------------------------------------------ */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 8 : *cap;
    while (new_cap < needed) {
        new_cap *= 2;
    }
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                /* out of memory */
    *arr = tmp;
    *cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Cached sequence storage                                           */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;   /* static cache array */
static size_t cache_len = 0;   /* number of elements stored (next free index) */
static size_t cache_cap = 0;   /* current capacity of cache */

/* ------------------------------------------------------------------ */
/* Main function                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 5) return 0;   /* values for indices 0–4 are 0 (by definition) */

    /* Initialise cache on first use */
    if (cache_len == 0) {
        /* store indices 0…7; cache_len = 8 */
        ensure_capacity(8, &cache_cap, &cache);
        for (idx_t i = 0; i <= 4; ++i) cache[i] = 0;  /* a(1)..a(5) zero? */
        cache[5] = (seq_t)1;     /* a(5) */
        cache[6] = (seq_t)3;     /* a(6) */
        cache[7] = (seq_t)10;    /* a(7) */
        cache_len = 8;
    }

    /* Extend cache up to requested index n */
    if ((size_t)n >= cache_len) {
        size_t needed = (size_t)n + 1;          /* include a(n) itself */
        ensure_capacity(needed, &cache_cap, &cache);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* Recurrence: a(i) = 2*a(i-1) + a(i-2) - 2*a(i-3)
                           + 2*a(i-4) + 2*a(i-5) + 5      (valid for i >= 8) */
            seq_t t1 = (seq_t)2 * cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = (seq_t)2 * cache[i - 4];
            seq_t t4 = (seq_t)2 * cache[i - 5];
            seq_t sum = t1 + t2 + t3 + t4 + (seq_t)5;
            seq_t sub = (seq_t)2 * cache[i - 3];
            cache[i] = sum - sub;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
