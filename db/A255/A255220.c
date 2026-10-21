/*
 * A255220  Number of (n+2)X(n+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 0 and no antidiagonal sum 0 and no row sum 2 and no column sum
 *          2.
 * https://oeis.org/A255220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A255220
   Recurrence: a(1)=23, a(2)=28, a(3)=46, a(4)=70, a(5)=106, a(6)=160
   For n>5:   a(n) = 2*a(n-1) – a(n-2) + a(n-3) – a(n-4)
   (All arithmetic performed in the build‑time type seq_t) */

#include <stdlib.h>

/* The function is required to have external linkage. */
seq_t a(idx_t n)
{
    /* Guard against calls with n==0 (undefined for the sequence). */
    if (n == 0) return (seq_t)0;

    /* Static cache of previously computed terms (1‑based). */
    static seq_t *cache      = NULL;
    static size_t cached_n   = 0;   /* number of terms currently valid */

    /* Allocate cache if this is the first call.  Ensure at least 6 slots. */
    if (cache == NULL) {
        size_t init_sz = (size_t)((n > 6) ? n : 6);
        cache = (seq_t *)malloc(init_sz * sizeof(seq_t));
        /* Initialise the first six values – the sequence is defined from a(1). */
        cache[0] = (seq_t)23;
        cache[1] = (seq_t)28;
        cache[2] = (seq_t)46;
        cache[3] = (seq_t)70;
        cache[4] = (seq_t)106;
        cache[5] = (seq_t)160;
        cached_n = 6;                /* valid terms are cache[0] … cache[5]   */
    }
    /* If n is smaller than the current cache size, the value is ready. */
    if (n <= cached_n) return cache[n - 1];

    /* Grow the cache to hold up to n terms. */
    size_t new_sz = (size_t)n;
    if (new_sz > cached_n) {
        cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        /* Compute missing terms using the recurrence.  The loop starts
           from the first index that is not yet computed (cached_n).  */
        for (size_t i = cached_n; i < new_sz; ++i) {
            /* i is zero‑based;  cache[i] corresponds to a(i+1). */
            seq_t term = (seq_t)2 * cache[i-1] - cache[i-2] + cache[i-3] - cache[i-4];
            cache[i] = term;
        }
        cached_n = new_sz;
    }

    return cache[n - 1];
}
