/*
 * A396311  Number of tilings of a 5 X n rectangle by unit squares and the
 *          Ferrers tile (3,2,1).
 * https://oeis.org/A396311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 1, a(3) = 4, a(4) = 8, a(5) = 15,
 *            a(6) = 31, a(7) = 63; a(n) = a(n-1) + 3*a(n-3) + a(n-4) +
 *            3*a(n-5) + a(n-7) for n ≥ 1, where a(k)=0 for k<0
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values.  */
static seq_t *cache = NULL;
/* Current capacity of the cache (number of elements).  */
static idx_t cap = 0;
/* Largest index already computed in the cache.  */
static idx_t last = 0;

/* Initialise cache with the first 8 terms and set capacity.  */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    /* Initial capacity at least 8.  */
    cap = 8;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (cache == NULL)
        exit(EXIT_FAILURE);  /* memory failed */
    /* First eight terms of the sequence.  */
    static const seq_t init[8] = {1, 1, 1, 4, 8, 15, 31, 63};
    for (idx_t i = 0; i < 8; ++i)
        cache[i] = init[i];
    last = 7;
}

/* Grow cache to at least n+1 elements.  New entries are initialised to 0.  */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cap)
        return;
    /* Double capacity until it is large enough.  */
    idx_t newcap = cap ? cap << 1 : 8;
    while (newcap <= n)
        newcap <<= 1;
    seq_t *newcache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (newcache == NULL)
        exit(EXIT_FAILURE);  /* memory failed */
    /* Zero new elements beyond the previously computed part.  */
    for (idx_t i = last + 1; i <= n && i < newcap; ++i)
        newcache[i] = 0;
    cache = newcache;
    cap = newcap;
}

/* Compute a(n) using memoisation.  */
seq_t a(idx_t n)
{
    init_cache();
    /* If the requested index is already cached, return it.  */
    if (n <= last)
        return cache[n];
    /* Extend cache to include n and compute new terms incrementally.  */
    ensure_capacity(n);
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t val = 0;
        /* a(k-1) */
        if (k >= 1)
            val += cache[k - 1];
        /* 3*a(k-3) */
        if (k >= 3)
            val += (seq_t)3 * cache[k - 3];
        /* a(k-4) */
        if (k >= 4)
            val += cache[k - 4];
        /* 3*a(k-5) */
        if (k >= 5)
            val += (seq_t)3 * cache[k - 5];
        /* a(k-7) */
        if (k >= 7)
            val += cache[k - 7];
        cache[k] = val;
    }
    last = n;
    return cache[n];
}
