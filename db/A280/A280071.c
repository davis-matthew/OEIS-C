/*
 * A280071  Indices of 11-gonal numbers (A051682) that are also centered
 *          11-gonal numbers (A060544).
 * https://oeis.org/A280071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 21*a(n-2) + a(n-3) (n>3) initial values: a(1)
 *            = 1, a(2) = 12, a(3) = 232
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <type_traits>
#include <vector>
#endif
#include <stdlib.h> /* realloc for C code */

static inline seq_t mult21(seq_t x)
{
#if defined(__cplusplus)
    if constexpr (std::is_integral<seq_t>::value) {
        /* use 128‑bit arithmetic for built‑in integer types */
        return (seq_t)((unsigned __int128)x * 21);
    } else {
        /* arbitrary‑precision types will multiply correctly */
        return x * 21;
    }
#else
    /* C build: seq_t is a plain unsigned integer type */
    return (seq_t)((unsigned __int128)x * 21);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1; undefined below 1 */

#if defined(__cplusplus)
    /* C++ build: use a static std::vector for memoisation */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(4);           /* indices 0..3, ignore 0 */
        cache[1] = 1;              /* a(1) */
        cache[2] = 12;             /* a(2) */
        cache[3] = 232;            /* a(3) */
    }
    if (cache.size() <= n) {
        size_t old = cache.size();
        cache.resize(n + 1);       /* ensure space up to a(n) */
        size_t start = (old >= 4) ? old : 4;
        for (size_t i = start; i <= n; ++i) {
            cache[i] = mult21(cache[i - 1]) - mult21(cache[i - 2]) + cache[i - 3];
        }
    }
    return cache[n];
#else
    /* C build: dynamic array with realloc */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* highest index computed so far */

    if (n > cache_max) {
        /* Expand the array to accommodate a(n) */
        size_t new_size = (size_t)n + 1;   /* indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;         /* allocation failure */
        cache = new_cache;

        /* Initialise initial values if they do not exist yet */
        if (new_size > 1) cache[1] = 1;   /* a(1) */
        if (new_size > 2) cache[2] = 12;  /* a(2) */
        if (new_size > 3) cache[3] = 232; /* a(3) */

        /* Compute missing terms using the recurrence */
        idx_t start = (cache_max >= 3) ? (cache_max + 1) : 4;
        for (idx_t i = start; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            cache[i] = mult21(t1) - mult21(t2) + t3;
        }
        cache_max = n;
    }
    return cache[n];
#endif
}
