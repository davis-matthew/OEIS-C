/*
 * A283330  a(n) = (1 + Sum_{j=1..K-1} a(n-j) + a(n-1)*a(n-K+1))/a(n-K) with
 *          a(1),...,a(K)=1, where K=5.
 * https://oeis.org/A283330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1..5) = 1, a(n) = (1 + a(n-1)+a(n-2)+a(n-3)+a(n-4) +
 *            a(n-1)*a(n-4)) / a(n-5) (K = 5)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 22, u64 n <= 40, u128 n <= 74,
 *            big any n
 * Helpers:   add5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/add5.h"

static seq_t *cache = NULL;   /* cache for computed terms */
static size_t cache_cap = 0;  /* allocated capacity */
static idx_t cache_len = 0;   /* number of terms already computed */

/* multiply two seq_t values with a 128‑bit intermediate if the type is integral,
   otherwise use normal multiplication (big‑integer types). */
static inline seq_t mul128(seq_t a, seq_t b)
{
#ifdef __cplusplus
    if constexpr (std::is_integral_v<seq_t>)
        return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
    else
        return a * b;
#else
    /* C path – seq_t must be an unsigned integral type */
    return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
#endif
}

static inline seq_t compute_next(idx_t i)
{
    /* i >= 6 */
    size_t ii = (size_t)i;   /* safe cast, i is unsigned */
    seq_t a0 = cache[ii - 2]; /* a(n-1) */
    seq_t a1 = cache[ii - 3]; /* a(n-2) */
    seq_t a2 = cache[ii - 4]; /* a(n-3) */
    seq_t a3 = cache[ii - 5]; /* a(n-4) */
    seq_t den = cache[ii - 6];/* a(n-5) */
#ifdef __cplusplus
    if constexpr (std::is_integral_v<seq_t>) {
        unsigned __int128 prod = (unsigned __int128)a0 * (unsigned __int128)a3;
        unsigned __int128 sum  = (unsigned __int128)a0 + (unsigned __int128)a1 +
                                 (unsigned __int128)a2 + (unsigned __int128)a3;
        unsigned __int128 numer = prod + sum + 1;
        return (seq_t)(numer / (unsigned __int128)den);
    } else {
        seq_t prod = a0 * a3;
        seq_t sum  = a0 + a1 + a2 + a3;
        seq_t numer = prod + sum + 1;
        return numer / den;
    }
#else
    unsigned __int128 prod = (unsigned __int128)a0 * (unsigned __int128)a3;
    unsigned __int128 sum  = (unsigned __int128)a0 + (unsigned __int128)a1 +
                             (unsigned __int128)a2 + (unsigned __int128)a3;
    unsigned __int128 numer = prod + sum + 1;
    return (seq_t)(numer / (unsigned __int128)den);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard against underflow */

    /* extend cache if needed */
    if ((idx_t)cache_len < n) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap < (size_t)n) new_cap *= 2;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 5) {
                cache[(size_t)(i - 1)] = (seq_t)1;
            } else {
                cache[(size_t)(i - 1)] = compute_next(i);
            }
        }
        cache_len = n;
    }
    return cache[(size_t)(n - 1)];
}
