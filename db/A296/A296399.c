/*
 * A296399  Number of n X 2 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 2 or 3 neighboring 1s.
 * https://oeis.org/A296399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 2*a(n-3) - 6*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) - a(n-7) - 2*a(n-8) + a(n-9)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

static const seq_t init_vals[9] = {1,4,8,17,46,114,289,757,1964};

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* For n ≤ 9 we can return immediately */
    if (n <= 9) return init_vals[(size_t)n-1];

#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static size_t computed = 0;

    /* Ensure the vector is large enough to index n */
    if (cache.size() <= (size_t)n) cache.resize(n + 1);

    /* Initialize the first nine terms once */
    if (computed == 0) {
        for (size_t i = 1; i <= 9; ++i) cache[i] = init_vals[i-1];
        computed = 9;
    }

    /* Compute any missing terms up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        seq_t pos = (seq_t)3 * cache[i-1] + (seq_t)2 * cache[i-3] +
                    (seq_t)3 * cache[i-5] + cache[i-9];
        seq_t neg = cache[i-2] + (seq_t)6 * cache[i-4] +
                    (seq_t)2 * cache[i-6] + cache[i-7] + (seq_t)2 * cache[i-8];
        cache[i] = pos - neg;
    }
    computed = (size_t)n;
    return cache[(size_t)n];
#else
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t computed = 0;

    /* Ensure enough capacity */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while ((size_t)n > new_cap) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        /* zero the newly allocated space */
        for (size_t i = cap + 1; i <= new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* Initialize the first nine terms once */
    if (computed == 0) {
        for (size_t i = 1; i <= 9; ++i) cache[i] = init_vals[i-1];
        computed = 9;
    }

    /* Compute any missing terms up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        seq_t pos = (seq_t)3 * cache[i-1] + (seq_t)2 * cache[i-3] +
                    (seq_t)3 * cache[i-5] + cache[i-9];
        seq_t neg = cache[i-2] + (seq_t)6 * cache[i-4] +
                    (seq_t)2 * cache[i-6] + cache[i-7] + (seq_t)2 * cache[i-8];
        cache[i] = pos - neg;
    }
    computed = (size_t)n;
    return cache[(size_t)n];
#endif
}
