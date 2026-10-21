/*
 * A298959  Number of nX4 0..1 arrays with every element equal to 3, 5, 6, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) + 2*a(n-4) - 2*a(n-5) initial values:
 *            a(1)=0, a(2)=2, a(3)=2, a(4)=7, a(5)=14
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;      /* array holding computed terms, 0-based */
static idx_t  cache_len = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity of the array */

/* Ensure the cache can hold at least 'need' elements. */
static void
ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = (cache_cap > 0 ? cache_cap << 1 : 8);
    if (new_cap < (size_t)need)
        new_cap = (size_t)need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);  /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Allocate space for requested index */
    ensure_cache(n);

    /* Initialisation: first five terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)2;   /* a(3) */
        cache[3] = (seq_t)7;   /* a(4) */
        cache[4] = (seq_t)14;  /* a(5) */
        cache_len = 5;
    }

    /* Return early if the requested term is already known */
    if (n <= cache_len)
        return cache[(size_t)(n - 1)];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Recurrence indices are 1-based; convert to 0-based offsets */
        seq_t a1 = cache[(size_t)(i - 2)]; /* a(i-1) */
        seq_t a3 = cache[(size_t)(i - 4)]; /* a(i-3) */
        seq_t a4 = cache[(size_t)(i - 5)]; /* a(i-4) */
        seq_t a5 = cache[(size_t)(i - 6)]; /* a(i-5) */

        seq_t val = a1 + a1;       /* 2*a(i-1) */
        val += a3;                 /* + a(i-3) */
        val += a4 + a4;            /* + 2*a(i-4) */
        val -= a5 + a5;            /* - 2*a(i-5) */

        cache[(size_t)(i - 1)] = val;
    }

    cache_len = n;
    return cache[(size_t)(n - 1)];
}
