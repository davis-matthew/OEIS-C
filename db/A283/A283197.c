/*
 * A283197  Number of n X 2 0..1 arrays with no 1 equal to more than two of
 *          its horizontal and vertical neighbors, with the exception of
 *          exactly one element.
 * https://oeis.org/A283197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) -5*a(n-2) -8*a(n-3) -18*a(n-4) -4*a(n-5) 
   +6*a(n-6) +8*a(n-7) +3*a(n-8) -2*a(n-9) - a(n-10)          */
/* Initial values (1‑based): 
   a(1)=0, a(2)=0, a(3)=6, a(4)=36, a(5)=176, a(6)=824, a(7)=3668,
   a(8)=15808, a(9)=66640, a(10)=276184 */
#include <stddef.h>
#include <stdlib.h>
#if !defined(__cplusplus)
# include <stdint.h>
#endif

static seq_t *cache = NULL;
static size_t cap   = 0; /* capacity of cache */
static size_t size  = 0; /* highest used index + 1 (size of stored terms) */

/* Ensure that the cache has space for at least 'need' elements */
static void extend_cache(size_t need)
{
    if (need <= cap)
        return;
    size_t new_cap = cap ? cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < size; ++i)
        new_cache[i] = cache[i];
    delete [] cache;
#else
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = new_cache;
    cap   = new_cap;
}

/* Initialize the known first ten terms */
static void init_cache(void)
{
    extend_cache(10);
    cache[0] = 0;  /* a(1) */
    cache[1] = 0;  /* a(2) */
    cache[2] = 6;  /* a(3) */
    cache[3] = 36; /* a(4) */
    cache[4] = 176;/* a(5) */
    cache[5] = 824;/* a(6) */
    cache[6] = 3668;/* a(7) */
    cache[7] = 15808;/* a(8) */
    cache[8] = 66640;/* a(9) */
    cache[9] = 276184;/* a(10) */
    size = 10;
}

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined index, return 0 as safe default */

    /* Initialise the cache with the first ten terms if it hasn't been done yet */
    if (size == 0)
        init_cache();

    /* If the requested term is already computed, return it directly */
    if (n <= size)
        return cache[n-1];

    /* Ensure cache can hold all terms up to n */
    extend_cache((size_t)n);

    /* Compute missing terms using the recurrence */
    for (idx_t i = size; i < n; ++i) { /* i is 0‑based index of a(i+1) */
        /* i >= 10 here, so all referenced indices exist */
        seq_t pos = 0; /* sum of positive‑coefficient contributions */
        seq_t neg = 0; /* sum of negative‑coefficient contributions */

        /* Positive contributions */
        pos += cache[i-1] * (seq_t)6;
        pos += cache[i-6] * (seq_t)6;
        pos += cache[i-7] * (seq_t)8;
        pos += cache[i-8] * (seq_t)3;

        /* Negative contributions */
        neg += cache[i-2] * (seq_t)5;
        neg += cache[i-3] * (seq_t)8;
        neg += cache[i-4] * (seq_t)18;
        neg += cache[i-5] * (seq_t)4;
        neg += cache[i-9] * (seq_t)2;
        neg += cache[i-10] * (seq_t)1;

        cache[i] = pos - neg;
    }
    size = n;
    return cache[n-1];
}
