/*
 * A282051  Number of n X 2 0..2 arrays with no element unequal to more than
 *          four of its king-move neighbors and with new values introduced in
 *          order 0 sequentially upwards.
 * https://oeis.org/A282051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 11*a(n-2) + 20*a(n-3) - 24*a(n-4) + 8*a(n-5)
 *            with initial values a(1)=2, a(2)=14, a(3)=91, a(4)=600,
 *            a(5)=4039
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of computed terms */
static seq_t *cache = NULL;          /* 0-based index: cache[0] = a(1) */
static size_t cache_size = 0;       /* allocated size */
static idx_t  max_computed = 0;     /* largest index (1‑based) computed */

/* ensure cache has capacity for n terms (n >= 1) */
static void ensure_capacity_to(idx_t n)
{
    if (cache_size < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (p == NULL) abort();      /* allocation failure */
        cache = p;
        /* zero new entries to avoid uninitialised reads */
        for (size_t i = cache_size; i < new_cap; ++i)
            cache[i] = 0;
        cache_size = new_cap;
    }
}

/* initialise the first five terms once */
static void initialise_first_five(void)
{
    if (max_computed > 0) return;
    if (cache_size < 5) {
        ensure_capacity_to(5);
    }
    cache[0] = (seq_t)2;
    cache[1] = (seq_t)14;
    cache[2] = (seq_t)91;
    cache[3] = (seq_t)600;
    cache[4] = (seq_t)4039;
    max_computed = 5;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* out of range, undefined */
    initialise_first_five();                   /* make sure we have 1‑5 ready */

    if ((idx_t)max_computed >= n) {             /* answer already known */
        return cache[n-1];
    }

    /* make sure the cache can hold up to n terms */
    ensure_capacity_to(n);

    /* compute terms from max_computed+1 up to n */
    for (idx_t i = (idx_t)max_computed; i < n; ++i) {
        /* i is 1‑based index of a(i+1). Use zero‑based array indices */
        idx_t k = i;                           /* k = i (zero‑based) */
        /* a(k+1) with k >=5 (since we start at i=5 when max_computed>=5) */
        seq_t term =
            8 * cache[k-1]          /* 8*a(k) */
            + 20 * cache[k-3]       /* +20*a(k-2) */
            + 8  * cache[k-5]       /* +8*a(k-4) */
            - 11 * cache[k-2]       /* -11*a(k-1) */
            - 24 * cache[k-4];      /* -24*a(k-3) */
        cache[k] = term;
    }
    max_computed = n;
    return cache[n-1];
}
