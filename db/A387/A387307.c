/*
 * A387307  a(n) = Sum_{k=0..n} 2^k * binomial(n+2,k+2) * binomial(2*k+4,k+4).
 * https://oeis.org/A387307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(n+2,k+2) *
 *            binomial(2*k+4,k+4) Recurrence: a(0)=1, a(1)=15 n*(n+4)*a(n) =
 *            (n+2)*(5*(2*n+3)*a(n-1) - 9*(n+1)*a(n-2)) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for memoised values */
static seq_t *cache     = NULL;   /* computed a(i) values */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of elements actually computed */

/* Ensure the cache has room for indices up to n and that all values are computed. */
static void ensure_cache(idx_t n)
{
    /* initialise if necessary */
    if (cache_len == 0) {
        /* allocate space for at least the first two elements */
        cache_cap = 2;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
        cache_len = 2;
    }

    /* grow the cache if required */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i <= 1) {            /* already initialised */
            continue;
        }
        /* previous two terms */
        seq_t a_nm1 = cache[i-1];
        seq_t a_nm2 = cache[i-2];

#ifdef __SIZEOF_INT128__
        /* use 128‑bit intermediate when seq_t is 64‑bit or smaller */
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            idx_t m   = i;
            __int128 X = (__int128)5 * (__int128)(2*m+3) * (__int128)a_nm1;
            __int128 Y = (__int128)9 * (__int128)(m+1)   * (__int128)a_nm2;
            __int128 numer = (__int128)(m+2) * (X - Y);
            __int128 denom = (__int128)m * (__int128)(m+4);
            cache[i] = (seq_t)(numer / denom);
            continue;
        }
#endif
        /* fall back to ordinary seq_t arithmetic (handles big‑int types) */
        seq_t X = 5;
        X *= (2*i + 3);
        X *= a_nm1;

        seq_t Y = 9;
        Y *= (i + 1);
        Y *= a_nm2;

        seq_t numer = (i + 2) * (X - Y);
        seq_t denom = i * (i + 4);
        cache[i] = numer / denom;
    }

    cache_len = (size_t)n + 1;
}

/* Public function required by the task */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
