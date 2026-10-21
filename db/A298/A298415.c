/*
 * A298415  a(n) = a(n-1) + 2*a(n-2) + a([n/2]), where a(0) = 1, a(1) = 1,
 *          a(2) = 1.
 * https://oeis.org/A298415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a([n/2]), with a(0)=1, a(1)=1, a(2)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache to store computed terms; grows as needed */
static seq_t *seq_cache = NULL;          /* pointer to array of seq_t */
static idx_t  seq_cache_len = 0;         /* number of terms currently stored (next free index) */
static size_t seq_cache_cap = 0;         /* allocated capacity in seq_t elements */

/* Ensure the cache has space for at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= seq_cache_cap)
        return;
    if (seq_cache_cap == 0)
        seq_cache_cap = 64;            /* starting capacity */
    while (seq_cache_cap < new_cap)
        seq_cache_cap <<= 1;           /* double until we can hold new_cap */
    /* realloc: cast required for C++ */
    seq_cache = (seq_t *)realloc(seq_cache, seq_cache_cap * sizeof(seq_t));
}

/* Fill the cache up to index n (inclusive). Assumes index n >= 0. */
static void compute_up_to(idx_t n)
{
    if (n < seq_cache_len)
        return;                        /* already computed */

    /* Allocate or grow buffer to hold at least n+1 terms */
    grow_cache((size_t)(n + 1));

    /* Initialise the first three terms if not already */
    if (seq_cache_len == 0) {
        seq_cache[0] = (seq_t)1;
        seq_cache_len = 1;
    }
    if (seq_cache_len == 1) {
        seq_cache[1] = (seq_t)1;
        seq_cache_len = 2;
    }
    if (seq_cache_len == 2) {
        seq_cache[2] = (seq_t)1;
        seq_cache_len = 3;
    }

    /* Compute subsequent terms in order */
    for (idx_t i = seq_cache_len; i <= n; ++i) {
        seq_t term = seq_cache[i-1] + seq_cache[i-2];
        term += seq_cache[i-2];                /* term = term + a(i-2) = a(i-1)+2*a(i-2) */
        term += seq_cache[i/2];
        seq_cache[i] = term;
    }
    seq_cache_len = n + 1;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return seq_cache[n];
}
