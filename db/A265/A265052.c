/*
 * A265052  Poincaré series for hyperbolic reflection group with Coxeter
 *          diagram shown in Comments.
 * https://oeis.org/A265052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 2*a(n-5) + 2*a(n-6) -
 *            2*a(n-8) + 3*a(n-9) - 3*a(n-10) + a(n-11) (n > 11) Initial
 *            values: a(0)=1, a(1)=4, a(2)=10, a(3)=22, a(4)=45, a(5)=89,
 *            a(6)=172, a(7)=328, a(8)=622, a(9)=1176, a(10)=2220, a(11)=4186
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 67, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifndef __cplusplus
#include <stdlib.h>
#endif

/* 12 known initial terms */
static const seq_t init[12] = {
    1ULL, 4ULL, 10ULL, 22ULL, 45ULL, 89ULL, 172ULL,
    328ULL, 622ULL, 1176ULL, 2220ULL, 4186ULL
};

#ifdef __cplusplus
/* C++ path – use a std::vector for automatic construction/destruction */
#include <vector>
static std::vector<seq_t> cache;
static size_t computed = 0;

/* Helper to ensure the vector has enough room */
static void ensure_cache_size(size_t n)
{
    if (n + 1 > cache.size())
        cache.resize(n + 1, 0);
}
#else
/* C path – use malloc/realloc and cast to seq_t* */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t computed = 0;

/* Helper to ensure the array has enough room */
static void ensure_cache_size(size_t n)
{
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_size = new_size;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n < 12)
        return init[(size_t)n];

    /* initialise cache on first use */
    if (computed == 0) {
#ifdef __cplusplus
        cache.resize(12);
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
#else
        cache_size = 12;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
#endif
        computed = 11;
    }

    /* Grow cache if needed */
    ensure_cache_size((size_t)n);

    /* Compute missing terms */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        seq_t res = 0;
        res += 3 * cache[i-1];
        res -= 3 * cache[i-2];
        res += 2 * cache[i-3];
        res -= 2 * cache[i-5];
        res += 2 * cache[i-6];
        res -= 2 * cache[i-8];
        res += 3 * cache[i-9];
        res -= 3 * cache[i-10];
        res += 1 * cache[i-11];
        cache[i] = res;
    }
    computed = (size_t)n;

    return cache[n];
}
