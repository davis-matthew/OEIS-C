/*
 * A259955  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 00000001 or 00001001.
 * https://oeis.org/A259955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) + 5*a(n-4) + 6*a(n-5) + 6*a(n-6) +
 *            2*a(n-7) for n > 8 Initial values: a(1) = 34, a(2) = 67, a(3) =
 *            155, a(4) = 353, a(5) = 808, a(6) = 1884, a(7) = 4340, a(8) =
 *            9925 The function computes the sequence using memoization so
 *            that any order of calls (for any n ≥ 1) is handled...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 103,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq_array.h"

/* Cached sequence values.  cache[0] corresponds to a(1).  */
static seq_t *cache = NULL;
static size_t cap  = 0;   /* current allocated size */
static idx_t  last = 0;   /* number of terms currently stored */

/* Helper to initialize the cache with the first 8 sequence terms.  */
static void init_cache(void)
{
    if (cache != NULL) return;          /* already initialized */
    grow_seq_array(&cache, &cap, 8);    /* allocate space for 8 terms */
    cache[0] = (seq_t)34;
    cache[1] = (seq_t)67;
    cache[2] = (seq_t)155;
    cache[3] = (seq_t)353;
    cache[4] = (seq_t)808;
    cache[5] = (seq_t)1884;
    cache[6] = (seq_t)4340;
    cache[7] = (seq_t)9925;
    last = 8;
}

/* Ensure that the cache contains terms up to at least n.  */
static void ensure_terms(idx_t n)
{
    if (n <= last) return;                  /* already computed */

    /* Grow the allocated array if necessary.  */
    if ((size_t)n > cap) {
        grow_seq_array(&cache, &cap, (size_t)n);
    }

    /* Compute new terms sequentially.  */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Convert 1‑based index i to 0‑based array positions.  */
        seq_t val = cache[(size_t)(i - 2)];                    /* a(i-1) */
        val += (seq_t)3 * cache[(size_t)(i - 4)];              /* 3*a(i-3) */
        val += (seq_t)5 * cache[(size_t)(i - 5)];              /* 5*a(i-4) */
        val += (seq_t)6 * cache[(size_t)(i - 6)];              /* 6*a(i-5) */
        val += (seq_t)6 * cache[(size_t)(i - 7)];              /* 6*a(i-6) */
        val += (seq_t)2 * cache[(size_t)(i - 8)];              /* 2*a(i-7) */
        cache[(size_t)(i - 1)] = val;                         /* store a(i) */
    }
    last = n;                                                /* update count */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0; return 0 to avoid UB */
    init_cache();
    ensure_terms(n);
    return cache[(size_t)(n - 1)];
}
