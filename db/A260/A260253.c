/*
 * A260253  Number of symmetry-allowed, linearly-independent terms at n-th
 *          order in the expansion of E x (e+a) rovibrational perturbation
 *          matrix H(Jx,Jy,Jz).
 * https://oeis.org/A260253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A260253 - Number of symmetry‑allowed, linearly‑independent terms.
   Recurrence: a(n) = 2*a(n-2) - a(n-4) + a(n-6) - 2*a(n-8) + a(n-10)
   Initial values (n = 0 … 10):
     1, 0, 4, 1, 9, 2, 16, 4, 25, 7, 36
   Computes a(n) for any n ≥ 0, caching computed values. */

#include <stddef.h>
#include <stdlib.h>

/* External types supplied by the build */
extern seq_t a(idx_t);

/* The implementation of a(n) */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  len    = 0;     /* number of filled entries */
    static size_t  cap    = 0;     /* allocated capacity            */

    /* initialise cache on first call -------------------------------------------------- */
    if (len == 0) {
        /* allocate initial capacity (at least size 11 for the given seed values) */
        cap = 16;                         /* arbitrary starting point */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* on allocation failure return zero */

        /* seed initial values (indices 0 … 10) ---------------------------------------- */
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 4;
        cache[3] = 1;
        cache[4] = 9;
        cache[5] = 2;
        cache[6] = 16;
        cache[7] = 4;
        cache[8] = 25;
        cache[9] = 7;
        cache[10] = 36;

        len = 11;   /* we have values up to index 10 inclusive */
    }

    /* cast index to size_t for array access --------------------------------------- */
    size_t idx = (size_t)n;

    /* if n less than current length we already have the value ------------------------ */
    if (idx < len) {
        return cache[idx];
    }

    /* grow cache so that it can hold n+1 elements ----------------------------------- */
    if (idx >= cap) {
        size_t new_cap = cap;
        while (new_cap <= idx) {
            /* double capacity each time; stop at required size */
            if (new_cap > SIZE_MAX / 2) {
                new_cap = idx + 1;
                break;
            }
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* on failure return zero */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms using the recurrence ------------------------------------ */
    for (size_t i = len; i <= idx; ++i) {
        seq_t v = cache[i - 2];
        v = v * 2;                /* 2*a(i-2) */
        v = v - cache[i - 4];     /* -a(i-4) */
        v = v + cache[i - 6];     /* +a(i-6) */
        v = v - (cache[i - 8] * 2);/* -2*a(i-8) */
        v = v + cache[i - 10];    /* +a(i-10) */
        cache[i] = v;
    }

    len = idx + 1; /* update length to include computed values */
    return cache[idx];
}
