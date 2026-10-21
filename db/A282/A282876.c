/*
 * A282876  Expansion of ((1 + 4*x + 8*x^2)^(3/2) - (1 + 6*x + 18*x^2 +
 *          20*x^3)) / (2*x^4) in powers of x.
 * https://oeis.org/A282876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies (n+4)*a(n)+2*(2*n+3)*a(n-1)+8*(n-1)*a(n-2)=0 with
 *            a(0)=3 and a(1)=-6
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 25, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>
#include <type_traits>
#endif

/* memoisation cache */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* grow cache to at least 'new_cap' elements */
static void grow_cache(size_t new_cap)
{
    if (cache_cap >= new_cap) return;
    size_t cap = cache_cap ? cache_cap * 2 : 16;
    if (cap < new_cap) cap = new_cap;
#if defined(__cplusplus)
    cache = reinterpret_cast<seq_t *>(realloc(cache, cap * sizeof(seq_t)));
#else
    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
#endif
    cache_cap = cap;
}

/* compute a(k) from a(k-1)=a1 and a(k-2)=a2 */
static inline seq_t compute_term(idx_t k, seq_t a1, seq_t a2)
{
#if defined(__cplusplus)
    if constexpr (std::is_integral<seq_t>::value) {
        /* use 128‑bit arithmetic when available */
        __int128 t1 = (__int128)a1 * (2 * k + 3) * 2;
        __int128 t2 = (__int128)a2 * (k - 1) * 8;
        __int128 numer = -(t1 + t2);
        return (seq_t)(numer / (k + 4));
    } else {
        seq_t t1 = (seq_t)2 * (seq_t)(2 * k + 3) * a1;
        seq_t t2 = (seq_t)8 * (seq_t)(k - 1) * a2;
        seq_t numer = -(t1 + t2);
        return numer / (seq_t)(k + 4);
    }
#else
#if defined(__SIZEOF_INT128__)
    __int128 t1 = (__int128)a1 * (2 * k + 3) * 2;
    __int128 t2 = (__int128)a2 * (k - 1) * 8;
    __int128 numer = -(t1 + t2);
    return (seq_t)(numer / (k + 4));
#else
    seq_t t1 = (seq_t)2 * (seq_t)(2 * k + 3) * a1;
    seq_t t2 = (seq_t)8 * (seq_t)(k - 1) * a2;
    seq_t numer = -(t1 + t2);
    return numer / (seq_t)(k + 4);
#endif
#endif
}

/* sequence function */
seq_t a(idx_t n)
{
    /* base values */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)-6;

    /* initialise cache */
    if (cache == NULL) {
        size_t init = 16;
        grow_cache(init);
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-6;
        cache_len = 2;
    }

    /* already computed ? */
    if (n < cache_len) return cache[n];

    /* extend cache up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        if (k >= cache_cap) grow_cache(k + 1);
        cache[k] = compute_term(k, cache[k - 1], cache[k - 2]);
    }
    cache_len = n + 1;
    return cache[n];
}
