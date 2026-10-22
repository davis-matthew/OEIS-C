/*
 * A384640  Expansion of (1-3*x^2) / (1-x-5*x^2+x^3+2*x^4).
 * https://oeis.org/A384640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = a(n-1) + 5*a(n-2) - a(n-3) - 2*a(n-4) , a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=7
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;      /* allocated array of a(k) values */
static size_t       cache_len   = 0;   /* number of allocated slots  */
static idx_t        last_computed = 0;  /* highest n whose a(n) is stored */

/* Ensure cache contains values up to n, computing missing ones */
static void compute_up_to(idx_t n)
{
    /* Allocate enough space */
    if (cache_len < (size_t)(n + 1)) {
        size_t new_len = cache_len ? cache_len << 1 : 16;
        if (new_len < (size_t)(n + 1))
            new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* Initialise first four terms if not yet done */
    if (last_computed < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        last_computed = 3;
    }

    /* Compute remaining terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = cache[i-1] + (seq_t)5 * cache[i-2];
        val -= cache[i-3];
        val -= (seq_t)2 * cache[i-4];
        cache[i] = val;
    }
    last_computed = n;
}

/* Public function returning a(n) */
seq_t a(idx_t n)
{
    if (n < 4) {
        /* return constant terms without using the cache */
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            case 2: return (seq_t)3;
            case 3: return (seq_t)7;
            default: return (seq_t)0; /* unreachable */
        }
    }
    compute_up_to(n);
    return cache[n];
}
