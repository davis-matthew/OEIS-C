/*
 * A279704  Number of n X 3 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A279704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=11, a(n)=5*a(n-1)-5*a(n-2)+2*a(n-3) (n≥4)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache to store already computed terms. */
static seq_t   *cache = NULL;   /* array of terms a(1) .. a(cache_len)          */
static size_t  cache_len  = 0;  /* number of terms currently stored           */
static size_t  cache_cap  = 0;  /* allocated capacity (in terms)               */

/* Ensure that the cache has space for at least 'need' terms (need ≥ 1). */
static void
ensure_capacity(idx_t need)
{
    if ((size_t)need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    if (new_cap < (size_t)need)
        new_cap = (size_t)need;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {                       /* allocation failed – abort silently   */
        return;
    }
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Return the n-th term (n ≥ 1).  Calls may arrive in any order. */
seq_t
a(idx_t n)
{
    /* Initialise the first three terms on the first call. */
    if (cache == NULL) {
        ensure_capacity(3);
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)3;   /* a(2) */
        cache[2] = (seq_t)11;  /* a(3) */
        cache_len = 3;
    }

    /* Handle requests for already known terms.  */
    if ((size_t)n <= cache_len) {
        return cache[(size_t)n - 1];
    }

    /* Expand cache and compute missing terms up to n. */
    ensure_capacity(n);
    for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i)
    {
        size_t idx  = (size_t)i - 1;                /* zero‑based index of a(i) */
        seq_t t1 = cache[idx - 1];                  /* a(i-1) */
        seq_t t2 = cache[idx - 2];                  /* a(i-2) */
        seq_t t3 = cache[idx - 3];                  /* a(i-3) */
        seq_t val = 5 * t1;
        val -= 5 * t2;
        val += 2 * t3;
        cache[idx] = val;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
