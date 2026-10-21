/*
 * A269963  Start with a square; at each stage add a square at each expandable
 *          vertex so that the ratio of the side of the squares at stage n+1
 *          and at stage n is the golden ratio phi=0.618...; a(n) is the
 *          number of squares in a portion of the n-th stage (see below).
 * https://oeis.org/A269963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-2*a(n-3)+2*a(n-4)+2*a(n-5) a(1)=1,
 *            a(2)=2, a(3)=7, a(4)=16, a(5)=37 (offset=1)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache: store values  a(1) at cache[0], a(2) at cache[1], ...   */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity   */
static idx_t  computed_max = 0;/* number of values computed (a(1)..a(computed_max)) */

/* enlarge cache to hold at least n elements (n>=1)   */
static void ensure_cache_size(idx_t n)
{
    if (n <= (idx_t)cache_cap) return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 64;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/*  A269963  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; 0 is unused */

    /* initialise cache once */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();         /* allocation failure */
    }

    ensure_cache_size(n);            /* make sure space for a(n) */

    /* initialise the initial five terms once */
    if (computed_max < 5) {
        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)2;          /* a(2) */
        cache[2] = (seq_t)7;          /* a(3) */
        cache[3] = (seq_t)16;         /* a(4) */
        cache[4] = (seq_t)37;         /* a(5) */
        computed_max = 5;
    }

    /* if already computed, return immediately */
    if (n <= computed_max) return cache[n-1];

    /* compute missing terms by recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* t1 = 2*(a(i-1) - a(i-3))  */
        seq_t t1 = cache[i-2] - cache[i-4];
        t1 = t1 + t1;                /* multiply by 2 */

        /* t2 = a(i-2) + 2*a(i-4) + 2*a(i-5) */
        seq_t t2 = cache[i-3];
        t2 += cache[i-5] + cache[i-5];
        t2 += cache[i-6] + cache[i-6];

        cache[i-1] = t1 + t2;        /* a(i) */
    }
    computed_max = n;
    return cache[n-1];
}
