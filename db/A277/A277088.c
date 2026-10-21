/*
 * A277088  Pisot sequences L(5,12), S(5,12).
 * https://oeis.org/A277088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277088: Pisot sequences L(5,12), S(5,12)      */
/* a(n) = ceil(a(n-1)^2 / a(n-2)),  a(0)=5, a(1)=12      */
/* The implementation memoises all computed terms and
   uses *128‑bit arithmetic* when available to avoid overflow
   for the 64‑bit build.                                      */

#include <stdlib.h>
#include <stdint.h>

/* Helper: compute ceil(a^2 / b) without intermediate overflow.  */
#if defined(__SIZEOF_INT128__) && !defined(_WIN32) && !defined(_MSC_VER)
/* 128‑bit intermediate for built‑in unsigned types.          */
static inline seq_t ceil_square_div(seq_t a, seq_t b)
{
    unsigned __int128 prod = (unsigned __int128) a * (unsigned __int128) a;
    seq_t  q = (seq_t)(prod / b);
    seq_t  r = (seq_t)(prod % b);
    return r == 0 ? q : q + 1;
}
#else
/* Fallback: works for arbitrary‑precision types as well.     */
static inline seq_t ceil_square_div(seq_t a, seq_t b)
{
    seq_t prod = a * a;
    seq_t q    = prod / b;
    seq_t r    = prod % b;
    return r == 0 ? q : q + 1;
}
#endif

/* Memoisation cache: holds a[0] … a[cache_len-1].           */
static seq_t *cache   = NULL;   /* pointer to sequence values   */
static size_t cache_len = 0;    /* number of valid entries stored */

/* Ensure cache can hold index n (i.e. at least n+1 elements). */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_len) return;   /* already large enough */

    size_t    new_len   = (size_t)(n + 1);
    seq_t *   new_cache = NULL;

#ifdef __cplusplus
    new_cache = new seq_t[new_len];
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    delete [] cache;                 /* older allocation */
#else
    new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) exit(1);         /* allocation failure */
#endif
    cache     = new_cache;
    cache_len = new_len;
}

/* Sequence function.   */
seq_t a(idx_t n)
{
    /* Initialise first two values once. */
    if (!cache)
    {
        cache_len = 2; /* store values 0 and 1 */
        cache = NULL;
#ifdef __cplusplus
        cache = new seq_t[cache_len];
#else
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(1);
#endif
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)12;
    }

    /* Direct lookup if already computed. */
    if ((size_t)n < cache_len) return cache[n];

    /* Remember old size before growing. */
    size_t old_len = cache_len;
    ensure_cache(n);

    /* Compute missing terms. */
    for (idx_t i = (idx_t)old_len; i <= n; ++i)
    {
        cache[i] = ceil_square_div(cache[i-1], cache[i-2]);
    }
    return cache[n];
}
