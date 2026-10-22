/*
 * A377310  Divisibility sequence associated with elliptic curve y^2 + y = x^3
 *          - x^2 - 2x + 2 and point (1, 0).
 * https://oeis.org/A377310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies the elliptic‑divisibility recurrence a(n) =
 *            [a(n-1)*a(n-3) – (a(n-2))^2] / a(n-4) for n ≥ 5 Initial values:
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=1, a(4)= -3 (negative indices obey
 *            a(–n) = –a(n))
 * Range:     i8 n <= 8, i16 n <= 11, i32 n <= 17, i64 n <= 24, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, memcpy */
#include <stdint.h>   /* uintptr_t */

static seq_t *cache = NULL;   /* cache of computed values */
static size_t  cap   = 0;     /* current cache capacity */
static idx_t   last  = 4;     /* largest computed index */

/* enlarge cache to hold at least (needed+1) elements */
static void ensure_capacity(idx_t needed)
{
    if ((size_t)(needed + 1) <= cap) return;
    size_t newcap = cap ? cap * 2 : 5;
    while ((size_t)(needed + 1) > newcap) newcap *= 2;
    seq_t *newcache = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newcache) abort();
    if (cache) { memcpy(newcache, cache, cap * sizeof(seq_t)); free(cache); }
    cache = newcache; cap = newcap;
}

/* initialize cache with the first five terms */
static void init_cache(void)
{
    if (cache == NULL) {
        cap   = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)-3;
        last   = 4;
    }
}

/* compute a(n) for any n >= 0; memoise results in cache */
seq_t a(idx_t n)
{
    init_cache();          /* ensure cache exists */
    ensure_capacity(n);    /* grow if necessary */

    if (n <= last) return cache[n];   /* already computed */

    /* compute terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        if (i < 5) {            /* should not happen, but guard */
            /* values already present in cache[0..4] */
            continue;
        }
        /* use 128‑bit temporary to avoid overflow */
        __int128 n1 = (__int128)cache[i-1];
        __int128 n3 = (__int128)cache[i-3];
        __int128 n2 = (__int128)cache[i-2];
        __int128 numer = n1 * n3 - n2 * n2;
        seq_t denom = cache[i-4];
        /* exact division: denom divides numer */
        cache[i] = (seq_t)(numer / denom);
    }
    last = n;                 /* update highest computed index */
    return cache[n];
}
