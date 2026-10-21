/*
 * A269049  Number of nX5 0..2 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A269049: Number of n×5 (0..2)-arrays with at most one adjacent
   pair (horizontal, diagonal, or anti‑diagonal) summing to two.
   Empirical recurrence (valid for n>13):
       a(n) = 28 a(n‑1) – 286 a(n‑2) + 1290 a(n‑3) – 2373 a(n‑4)
              + 304 a(n‑5) + 3551 a(n‑6) – 2846 a(n‑7) – 546 a(n‑8)
              + 1308 a(n‑9) – 505 a(n‑10) + 76 a(n‑11) – 4 a(n‑12)
   Initial values (a(1) through a(13)):
       144, 1068, 13140, 149358, 1643376, 17696520, 187575858,
       1964080920, 20365312416, 209472681102, 2140087086852,
       21738862087680, 219728193571542                                                             */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Cache for computed terms */
static seq_t *cache = NULL;           /* 0‑based, index 0 unused */
static size_t cache_cap = 0;           /* allocated size (number of slots) */
static idx_t  cache_last = 0;          /* highest index that has a valid value */

/* Ensure cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (cache_cap ? cache_cap * 2 : (size_t)n + 2);
        if (new_cap < (size_t)n + 2) new_cap = (size_t)n + 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Initial 13 terms */
static const seq_t init[13] = {
    (seq_t)144ULL,
    (seq_t)1068ULL,
    (seq_t)13140ULL,
    (seq_t)149358ULL,
    (seq_t)1643376ULL,
    (seq_t)17696520ULL,
    (seq_t)187575858ULL,
    (seq_t)1964080920ULL,
    (seq_t)20365312416ULL,
    (seq_t)209472681102ULL,
    (seq_t)2140087086852ULL,
    (seq_t)21738862087680ULL,
    (seq_t)219728193571542ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    ensure_cache(n);

    /* Supply the first 13 values if not yet present */
    if (cache_last < 13) {
        idx_t to = (n < 13 ? n : 13);
        for (idx_t i = 1; i <= to; ++i) cache[i] = init[i-1];
        if (to == 13) cache_last = 13;
        else {
            /* If n < 13 we may finish here */
            cache_last = to;
            return cache[n];
        }
    }

    /* Compute terms up to n using the recurrence (for i>13) */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
#ifdef __cplusplus
        /* In C++ we prefer std::is_arithmetic to choose the right
           implementation.  If seq_t is a normal integral type we use
           128‑bit intermediate arithmetic; otherwise we rely on seq_t
           operations. */
        if constexpr (std::is_arithmetic<seq_t>::value && sizeof(seq_t) <= 8) {
            unsigned __int128 pos = 0;
            unsigned __int128 neg = 0;
            pos += (unsigned __int128)cache[i-1] * 28;
            pos += (unsigned __int128)cache[i-3] * 1290;
            pos += (unsigned __int128)cache[i-5] * 304;
            pos += (unsigned __int128)cache[i-6] * 3551;
            pos += (unsigned __int128)cache[i-9] * 1308;
            pos += (unsigned __int128)cache[i-11] * 76;
            neg += (unsigned __int128)cache[i-2] * 286;
            neg += (unsigned __int128)cache[i-4] * 2373;
            neg += (unsigned __int128)cache[i-7] * 2846;
            neg += (unsigned __int128)cache[i-8] * 546;
            neg += (unsigned __int128)cache[i-10] * 505;
            neg += (unsigned __int128)cache[i-12] * 4;
            cache[i] = (seq_t)(pos - neg);
        } else {
            seq_t pos = 0;
            seq_t neg = 0;
            pos += cache[i-1] * (seq_t)28;
            pos += cache[i-3] * (seq_t)1290;
            pos += cache[i-5] * (seq_t)304;
            pos += cache[i-6] * (seq_t)3551;
            pos += cache[i-9] * (seq_t)1308;
            pos += cache[i-11] * (seq_t)76;
            neg += cache[i-2] * (seq_t)286;
            neg += cache[i-4] * (seq_t)2373;
            neg += cache[i-7] * (seq_t)2846;
            neg += cache[i-8] * (seq_t)546;
            neg += cache[i-10] * (seq_t)505;
            neg += cache[i-12] * (seq_t)4;
            cache[i] = pos - neg;
        }
#else
        /* Pure C implementation using 128‑bit intermediate arithmetic */
        unsigned __int128 pos = 0;
        unsigned __int128 neg = 0;
        pos += (unsigned __int128)cache[i-1] * 28;
        pos += (unsigned __int128)cache[i-3] * 1290;
        pos += (unsigned __int128)cache[i-5] * 304;
        pos += (unsigned __int128)cache[i-6] * 3551;
        pos += (unsigned __int128)cache[i-9] * 1308;
        pos += (unsigned __int128)cache[i-11] * 76;
        neg += (unsigned __int128)cache[i-2] * 286;
        neg += (unsigned __int128)cache[i-4] * 2373;
        neg += (unsigned __int128)cache[i-7] * 2846;
        neg += (unsigned __int128)cache[i-8] * 546;
        neg += (unsigned __int128)cache[i-10] * 505;
        neg += (unsigned __int128)cache[i-12] * 4;
        cache[i] = (seq_t)(pos - neg);
#endif
    }
    cache_last = n;
    return cache[n];
}
