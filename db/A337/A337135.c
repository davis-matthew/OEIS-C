/*
 * A337135  a(1) = 1; for n > 1, a(n) = Sum_{d|n, d <= sqrt(n)} a(d).
 * https://oeis.org/A337135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8803, u16 n <= 8803, u32 n <= 8803, u64 n <= 8803,
 *            u128 n <= 8803, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence (OEIS A337135)
    a(1) = 1
    a(n) = Σ a(d)   for all d | n with d ≤ √n   (n > 1)
*/

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* ------------------------------------------------------------------ */
/* Integer square‑root for 64‑bit unsigned values.                    */
/* Returns the greatest x with x*x ≤ n.                               */
/* ------------------------------------------------------------------ */
static idx_t
floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;           /* > sqrt(2^64-1) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)n)
            return mid;
        else if (sq < (unsigned __int128)n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;                           /* hi is the floor sqrt */
}

/* ------------------------------------------------------------------ */
/*  Memoised cache for a(n).  The cache grows as needed.              */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;   /* index i used directly (1‑based) */
static idx_t  cache_size = 0;     /* last index that can be accessed  */

/* Expand the cache array to dimension 'n+1', preserving existing      */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    idx_t    new_size = n + 1;
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_size];      /* default‑constructed objects   */
#else
    new_cache = (seq_t *)calloc((size_t)new_size, sizeof(seq_t));
#endif
    /* Copy old entries and free old storage */
    if (cache) {
        for (idx_t i = 1; i <= cache_size; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }
    cache = new_cache;
    cache_size = n;
}

/* ------------------------------------------------------------------ */
/*  Sequence function                                                    */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    /* Base case */
    if (n == 1) {
        ensure_cache(1);
        if (cache[1] == (seq_t)0)
            cache[1] = (seq_t)1;
        return cache[1];
    }

    /* Cached result? */
    if (n <= cache_size && cache[n] != (seq_t)0)
        return cache[n];

    /* Compute and cache */
    ensure_cache(n);
    seq_t sum = (seq_t)0;
    idx_t lim = floor_sqrt_u64(n);
    for (idx_t d = 1; d <= lim; ++d) {
        if (n % d == 0)
            sum += a(d);
    }
    cache[n] = sum;
    return sum;
}
