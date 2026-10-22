/*
 * A330383  a(n) = (a(n-1)*a(n-7) + a(n-2)*a(n-6) - a(n-3)*a(n-5) + a(n-4)^2)
 *          / a(n-8), a(0) = ... = a(7) = 1.
 * https://oeis.org/A330383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 20, u32 n <= 27, u64 n <= 38, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (a(n-1)*a(n-7) + a(n-2)*a(n-6) - a(n-3)*a(n-5) + a(n-4)^2) / a(n-8)
 * Initial values: a(0)=a(1)=...=a(7)=1
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* allocated size of cache     */
static idx_t  next_cache = 0;   /* first index not yet computed */

/* make sure cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);   /* out‑of‑memory */
    cache = tmp;
    cache_size = new_size;
}

/* compute a single new term; implementation chosen at compile time */
#ifdef __cplusplus
template <typename T>
static inline void compute_term(idx_t i, T *c)
{
    if constexpr (std::is_integral<T>::value) {
        /* use 128‑bit intermediates for integer types */
        unsigned __int128 t1 = (unsigned __int128)c[i-1] * (unsigned __int128)c[i-7];
        unsigned __int128 t2 = (unsigned __int128)c[i-2] * (unsigned __int128)c[i-6];
        unsigned __int128 t3 = (unsigned __int128)c[i-3] * (unsigned __int128)c[i-5];
        unsigned __int128 t4 = (unsigned __int128)c[i-4] * (unsigned __int128)c[i-4];
        unsigned __int128 num = t1 + t2 + t4 - t3;
        c[i] = (T)(num / (unsigned __int128)c[i-8]);
    } else {
        /* for a big‑integer class use its own operations */
        c[i] = c[i-1] * c[i-7] + c[i-2] * c[i-6]
             - c[i-3] * c[i-5] + c[i-4] * c[i-4];
        c[i] /= c[i-8];
    }
}
#else
/* C implementation – seq_t is always an integral type */
static inline void compute_term(idx_t i, seq_t *c)
{
    unsigned __int128 t1 = (unsigned __int128)c[i-1] * (unsigned __int128)c[i-7];
    unsigned __int128 t2 = (unsigned __int128)c[i-2] * (unsigned __int128)c[i-6];
    unsigned __int128 t3 = (unsigned __int128)c[i-3] * (unsigned __int128)c[i-5];
    unsigned __int128 t4 = (unsigned __int128)c[i-4] * (unsigned __int128)c[i-4];
    unsigned __int128 num = t1 + t2 + t4 - t3;
    c[i] = (seq_t)(num / (unsigned __int128)c[i-8]);
}
#endif

seq_t a(idx_t n)
{
    if (n <= 7)                          /* base values */
        return 1;

    ensure_cache(n);

    /* initialise the first eight terms if not done yet */
    if (next_cache <= 7) {
        for (idx_t j = next_cache; j <= 7; ++j)
            cache[j] = 1;
        next_cache = 8;
    }

    for (idx_t i = next_cache; i <= n; ++i)
        compute_term(i, cache);

    next_cache = n + 1;
    return cache[n];
}
