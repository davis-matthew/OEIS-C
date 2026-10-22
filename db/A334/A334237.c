/*
 * A334237  a(n) = 2*Sum_{k=0..n-1} binomial(n,k)^2*binomial(n,k+1)^2.
 * https://oeis.org/A334237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334237: a(n) = 2*Sum_{k=0}^{n-1} binomial(n,k)^2*binomial(n,k+1)^2
   (offset 1).  Computed by the known linear recurrence:
   (n-1)^2*(n+1)^3*(5*n^2-10*n+4)*a(n) =
        2*n^2*(2*n-1)*(15*n^4-30*n^3+7*n^2+8*n-8)*a(n-1) +
        4*(n-1)^2*n*(4*n-5)*(4*n-3)*(5*n^2-1)*a(n-2)
   Initial values: a(1)=2, a(2)=16.  The implementation uses 128‑bit
   intermediate arithmetic to avoid overflow while retaining exact
   integer results for all n whose value fits in seq_t.  Results are
   memoised. */

#include <stddef.h>
#include <stdlib.h>

/* Public function required by the task */
seq_t a(idx_t n);

/* ----------  internal cache  ---------- */
static seq_t *cache = NULL;       /* 1‑based, index 0 unused */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t max_computed = 0;   /* largest n already computed */

/* Ensure the cache has room for indices 0…n (index 0 not used). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while ((size_t)n >= new_cap) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache at the first call of a(). */
static void init_cache(void)
{
    if (cache) return;          /* already initialised */

    cache_cap = 8;               /* minimal capacity (0..7) */
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (!cache) exit(1);

    cache[1] = (seq_t)2;        /* a(1) */
    cache[2] = (seq_t)16;       /* a(2) */
    max_computed = 2;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined, but return 0 */

    init_cache();
    ensure_cache(n);

    /* compute any missing terms up to n */
    for (size_t idx = max_computed + 1; idx <= (size_t)n; ++idx)
    {
        /* 128‑bit local variables for intermediate calculations */
        __int128 ni   = (__int128)idx;          /* n as 128 bit */
        __int128 n2   = ni * ni;                /* n^2 */
        __int128 n3   = n2 * ni;                /* n^3 */
        __int128 n4   = n2 * n2;                /* n^4 */

        /* polynomials appearing in the recurrence */
        __int128 poly1 = 15 * n4 - 30 * n3 + 7 * n2 + 8 * ni - 8;
        __int128 poly2 = 5 * n2 - 10 * ni + 4;
        __int128 poly3 = 5 * n2 - 1;

        __int128 nm1  = ni - 1;                   /* n-1 */
        __int128 np1  = ni + 1;                   /* n+1 */

        /* denominator: (n-1)^2 * (n+1)^3 * poly2 */
        __int128 denom = nm1 * nm1;                          /* (n-1)^2 */
        denom *= np1 * np1 * np1;                           /* * (n+1)^3 */
        denom *= poly2;                                     /* * poly2 */

        /* first numerator part: 2*n^2*(2*n-1)*poly1*a(n-1) */
        __int128 t1 = 2 * n2;                              /* 2*n^2 */
        t1 *= (2 * ni - 1);                                /* * (2*n-1) */
        t1 *= poly1;                                       /* * poly1 */
        t1 *= (__int128)cache[idx - 1];                    /* * a(n-1) */

        /* second numerator part: 4*(n-1)^2*n*(4*n-5)*(4*n-3)*poly3*a(n-2) */
        __int128 t2 = 4 * nm1 * nm1;                       /* 4*(n-1)^2 */
        t2 *= ni;                                          /* * n */
        t2 *= (4 * ni - 5);                                /* * (4*n-5) */
        t2 *= (4 * ni - 3);                                /* * (4*n-3) */
        t2 *= poly3;                                       /* * poly3 */
        t2 *= (__int128)cache[idx - 2];                  /* * a(n-2) */

        __int128 num = t1 + t2;                            /* sum of numerators */

        /* division is exact; final result fits in seq_t for small n */
        seq_t val = (seq_t)(num / denom);
        cache[idx] = val;
    }

    max_computed = (size_t)n;
    return cache[n];
}
