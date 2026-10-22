/*
 * A373959  Number of compositions of 6*n-4 into parts 1 and 6.
 * https://oeis.org/A373959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of compositions of 6*n-4 into parts 1 and 6.
 *            Recurrence (for n ≥ 7): a(n) = 7*a(n-1) - 15*a(n-2) + 20*a(n-3)
 *            - 15*a(n-4) + 6*a(n-5) - a(n-6) Initial values: a(1)=1, a(2)=4,
 *            a(3)=16, a(4)=71, a(5)=322, a(6)=1455 Indexed as OEIS: a(1) is
 *            the first term
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed terms.  Index 0 is unused.  */
static seq_t *cache = NULL;     /* pointer to array of seq_t   */
static idx_t  cache_cnt = 0;    /* largest n computed so far  */
static size_t cache_cap = 0;    /* allocated capacity (>= cache_cnt+1) */

/* Initial values for the first six terms.  Index 0 stays unused. */
static const seq_t init_vals[7] = {
    (seq_t)0,   /* dummy for index 0 */
    (seq_t)1,   /* a(1)  */
    (seq_t)4,   /* a(2)  */
    (seq_t)16,  /* a(3)  */
    (seq_t)71,  /* a(4)  */
    (seq_t)322, /* a(5)  */
    (seq_t)1455 /* a(6)  */
};

/* Ensure that a(1..n) are stored in the cache.  Expands the cache as necessary. */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_cnt)        /* already computed */
        return;

    /* Determine new capacity: must hold indices 0..n (so capacity >= n+1).  */
    size_t new_cap = (size_t)n + 1;
    if (new_cap < 7)           /* at least indices 1..6 */
        new_cap = 7;

    /* Allocate or grow the cache.  */
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* unrecoverable failure */
        cache_cap = new_cap;
    } else if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();            /* unrecoverable failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill in any newly needed indices.  */
    for (idx_t i = cache_cnt + 1; i <= n; ++i) {
        if (i <= 6) {
            /* initial values for the first six terms */
            cache[i] = init_vals[i];
        } else {
            /* recurrence: use the six previous terms */
            seq_t p1 = cache[i - 1];
            seq_t p2 = cache[i - 2];
            seq_t p3 = cache[i - 3];
            seq_t p4 = cache[i - 4];
            seq_t p5 = cache[i - 5];
            seq_t p6 = cache[i - 6];

            seq_t term = (seq_t)7 * p1;
            term = term - (seq_t)15 * p2;
            term = term + (seq_t)20 * p3;
            term = term - (seq_t)15 * p4;
            term = term + (seq_t)6  * p5;
            term = term - p6;

            cache[i] = term;
        }
    }
    cache_cnt = n;
}

/* Public function returning a(n) for n ≥ 1.  Caller may invoke in any order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for OEIS sequence, harmless */
    ensure_cache(n);
    return cache[n];
}
