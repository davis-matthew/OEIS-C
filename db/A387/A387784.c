/*
 * A387784  Lower (2/3)-midsequence of (F(2n)) and (F(2n+1)), where F=A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A387784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - a(n-2) + a(n-4) - 3*a(n-5) + a(n-6) */
/* Initial values: a(0)=0, a(1)=2, a(2)=5, a(3)=14, a(4)=36, a(5)=96 */

#include <stdlib.h>

static const seq_t a_init[6] = {0, 2, 5, 14, 36, 96};

static seq_t *a_cache = NULL;   /* cache of computed terms   */
static idx_t   cache_max = 0;   /* highest index stored      */
static size_t  cache_cap = 0;   /* allocated size of cache   */

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (n <= cache_max) return;

    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 6;                 /* start with the initial capacity   */
    while (new_cap <= (size_t)n) new_cap <<= 1;    /* grow exponentially                */

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                            /* allocation failed */
    a_cache = tmp;
    cache_cap = new_cap;
    /* new entries will be filled during the recurrence loop   */
}

/* External function returning a(n) */
extern seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n < 6) return a_init[n];

    /* Initialise cache on first invocation */
    if (!a_cache) {
        a_cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!a_cache) abort();
        for (idx_t i = 0; i < 6; ++i) a_cache[i] = a_init[i];
        cache_cap = 6;
        cache_max = 5;
    }

    ensure_cache_size(n);

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t t = 3 * a_cache[i-1];          /* 3*a(n-1) */
        t -= a_cache[i-2];                  /* - a(n-2) */
        t += a_cache[i-4];                  /* + a(n-4) */
        t -= 3 * a_cache[i-5];              /* - 3*a(n-5) */
        t += a_cache[i-6];                  /* + a(n-6) */
        a_cache[i] = t;                     /* store result */
    }

    cache_max = n;
    return a_cache[n];
}
