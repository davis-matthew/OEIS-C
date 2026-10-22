/*
 * A364641  G.f. satisfies A(x) = 1/(1 - 2*x) - x*A(x)^3.
 * https://oeis.org/A364641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 2^n - sum_{i+j+k=n-1} a[i]a[j]a[k] (n >= 1)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses incremental convolutions to compute the cubic
   sum efficiently in O(n^2) time.  The cache is lazily expanded so
   that calls to a(n) with arbitrary n order are supported. */

#include <stdlib.h>

/* local helper: grow a dynamic array to hold at least `needed` elements. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap > 0) ? *cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    /* initialise new elements to zero */
    for (size_t i = *cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;
    *ptr = tmp;
    *cap = new_cap;
}

/* static storage for the cache */
static seq_t *a_cache   = NULL;  /* a[n] values   */
static seq_t *b_cache   = NULL;  /* b[n] = sum_{i+j=n} a[i]a[j] */
static seq_t *pow2_cache = NULL; /* 2^n           */

static size_t a_cap       = 0;
static size_t b_cap       = 0;
static size_t pow2_cap    = 0;
static idx_t  cache_max   = 0;   /* last computed index */

/* expand the caches to accommodate positions up to `n`. */
static void ensure_cache_size(idx_t n)
{
    /* allocate/expand arrays */
    ensure_capacity((size_t)n + 1, &a_cap, &a_cache);
    ensure_capacity((size_t)n + 1, &pow2_cap, &pow2_cache);
    ensure_capacity((size_t)2 * n + 1, &b_cap, &b_cache);

    /* initialise the base case if not yet done */
    if (cache_max == 0 && a_cache[0] == (seq_t)0)
    {
        a_cache[0] = (seq_t)1;                 /* a(0) = 1          */
        pow2_cache[0] = (seq_t)1;              /* 2^0 = 1           */
        b_cache[0] = (seq_t)1;                 /* b(0) = a0^2 = 1   */
        cache_max = 0;                         /* last computed index*/
    }

    /* extend the sequence up to n */
    for (idx_t m = cache_max + 1; m <= n; ++m)
    {
        /* 2^m = 2^(m-1) * 2 */
        pow2_cache[m] = pow2_cache[m - 1] * (seq_t)2;

        /* cubic convolution sum for m-1: c = sum_{k=0}^{m-1} a[k]*b[(m-1)-k] */
        seq_t c = (seq_t)0;
        for (idx_t k = 0; k <= m - 1; ++k)
            c += a_cache[k] * b_cache[(m - 1) - k];

        /* a[m] = 2^m - c */
        a_cache[m] = pow2_cache[m] - c;

        /* update b so that it contains convolution of a[0..m] with itself.
           For i from 0 to m-1 add 2*a[m]*a[i] to b[m+i] (pairs (m,i) and (i,m)).
           For i=m add a[m]*a[m] once to b[2*m] (pair (m,m)). */
        for (idx_t i = 0; i < m; ++i)
            b_cache[m + i] += (a_cache[m] * a_cache[i]) * (seq_t)2;
        b_cache[2 * m] += a_cache[m] * a_cache[m];
    }

    cache_max = n;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n < cache_max + 1 && a_cache && a_cache[n] != (seq_t)0)
        return a_cache[n];
    ensure_cache_size(n);
    return a_cache[n];
}
