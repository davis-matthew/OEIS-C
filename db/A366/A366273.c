/*
 * A366273  G.f. A(x) satisfies A(x) = 1 + x*(1 + x)^4*A(x)^5.
 * https://oeis.org/A366273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*k, n-k) * binomial(5*k, k) /
 *            (4*k+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for any n ≥ 0.  Values are cached for repeated calls.  */

/* ------------------------------------------------------------------ */
#include <stddef.h>   /* size_t   */
#include <stdlib.h>   /* malloc, realloc */

/* ------------------------------------------------------------------ */
/*  Helper:  binomial( n , k ) using a multiplicative method.       */
/*  Intermediate products are performed in unsigned __int128 to avoid */
/*  overflow for the 64‑bit build.                                     */
/* ------------------------------------------------------------------ */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;

    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        idx_t num = n - k + i;
        unsigned __int128 temp = (unsigned __int128)result * (unsigned __int128)num;
        temp /= (unsigned __int128)i;
        result = (seq_t)temp;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/*  Ensure the cache has at least 'mincap' elements.  Resize by      */
/*  doubling to keep amortised O(1).  The cache array holds a(0) … a(max‑1). */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t mincap, seq_t **cache, size_t *cap)
{
    if (*cache == NULL) {
        *cap = mincap < 64 ? 64 : (size_t)mincap;
        *cache = (seq_t *)malloc(*cap * sizeof(seq_t));
    } else if (mincap > *cap) {
        size_t newcap = *cap;
        while (newcap < mincap) newcap *= 2;
        *cache = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Static cache of computed a(0)…a(max‑1) */
    static seq_t *cache = NULL;
    static idx_t  max_computed = 0;     /* first index not yet computed */
    static size_t cache_cap    = 0;

    /* Resize cache if necessary. */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* Compute missing entries up to n. */
    for (idx_t i = max_computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        for (idx_t k = (idx_t)0; k <= i; ++k) {
            seq_t c1 = binomial((idx_t)(4 * k), i - k);
            seq_t c2 = binomial((idx_t)(5 * k), k);
            /* (c1 * c2) is exact; division by (4*k+1) produces an integer. */
            unsigned __int128 prod = (unsigned __int128)c1 * (unsigned __int128)c2;
            prod /= (unsigned __int128)(4 * k + 1);
            sum += (seq_t)prod;
        }
        cache[i] = sum;
    }
    max_computed = n + 1;
    return cache[n];
}
