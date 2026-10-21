/*
 * A250784  Number of (2+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - a(n-3) + 2*a(n-4) Initial values:
 *            a(1)=18, a(2)=46, a(3)=106, a(4)=238
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array storing already computed terms */
static idx_t cache_len = 0;          /* highest index (n) for which cache[n-1] is valid */
static size_t cache_cap = 0;         /* allocated capacity of cache array */

/* Ensure cache contains terms up to and including n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialise with the first four terms */
        cache_cap = 4;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;          /* silent failure, caller will handle */
        cache[0] = (seq_t)18;
        cache[1] = (seq_t)46;
        cache[2] = (seq_t)106;
        cache[3] = (seq_t)238;
        cache_len = 4;
    }

    if (n <= cache_len) return;          /* already computed */

    if ((idx_t)cache_cap < n) {           /* grow the array if necessary */
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap < n) new_cap <<= 1;   /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                 /* silent failure, caller will handle */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* compute missing terms using the linear recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* indices of the recurrence terms:
           a(i-1) -> cache[i-2]
           a(i-2) -> cache[i-3]
           a(i-3) -> cache[i-4]
           a(i-4) -> cache[i-5]
        */
        seq_t term = cache[i-2];          /* 4*a(i-1) part will be added later */
        term = term * (seq_t)4;           /* 4*a(i-1) */
        term += cache[i-5] * (seq_t)2;    /* + 2*a(i-4) */
        term -= cache[i-3] * (seq_t)4;    /* - 4*a(i-2) */
        term -= cache[i-4];               /* - a(i-3) */
        cache[i-1] = term;
    }
    cache_len = n;
}

/* External function returning the nth term (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour for n==0 is undefined but harmless */

    ensure_cache(n);
    return cache[n-1];
}
