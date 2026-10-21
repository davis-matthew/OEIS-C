/*
 * A297521  Number of 3Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 3 neighboring 1s.
 * https://oeis.org/A297521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A297521: Number of 3×n 0..1 arrays with every 1 horizontally,
   diagonally or anti‑diagonally adjacent to 1 or 3 neighboring 1s.
   Empirical recurrence (valid for n ≥ 13):
   a(n) = a(n-1)+a(n-2)+10*a(n-3)+37*a(n-4)+23*a(n-5)+21*a(n-6)
          -25*a(n-7)-40*a(n-8)-44*a(n-9)+17*a(n-11)+5*a(n-12)
   Initial values:
   a(1)=1, a(2)=11, a(3)=49, a(4)=128, a(5)=363, a(6)=1408,
   a(7)=5079, a(8)=16104, a(9)=52308, a(10)=180643, a(11)=616299,
   a(12)=2045926. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms, 1‑based indexing */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;    /* allocated capacity */
    static size_t cache_len = 0;    /* highest index with a value */

    /* initial values for 1 ≤ i ≤ 12 */
    const seq_t init_vals[13] = {
        0,          /* dummy for index 0 */
        1, 11, 49, 128, 363, 1408,
        5079, 16104, 52308, 180643, 616299, 2045926
    };

    /* first call: allocate and load the base values */
    if (cache == NULL) {
        /* start with a modest capacity; will grow as needed */
        cache_cap = 32;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_cap + 1)); /* +1 so index == cache_cap is valid */
        if (!cache) return 0;          /* out of memory – caller should handle */
        /* initialise whole array to zero first */
        for (size_t i = 0; i <= cache_cap; ++i) cache[i] = 0;
        /* copy the known initial values */
        for (size_t i = 1; i <= 12; ++i) cache[i] = init_vals[i];
        cache_len = 12;
    }

    /* trivial case: n == 0 or n within the known range */
    if (n == 0) return 0;
    if ((size_t)n <= cache_len) return cache[(size_t)n];

    /* extend the cache to reach at least index n */
    size_t needed = (size_t)n;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp) return 0;          /* out of memory – caller should handle */
        /* initialise the newly allocated part */
        for (size_t i = cache_cap + 1; i <= new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* iterate forward, using the recurrence */
    for (size_t i = cache_len + 1; i <= needed; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* positive contributions */
        pos += cache[i-1];
        pos += cache[i-2];
        pos += cache[i-3] * 10;
        pos += cache[i-4] * 37;
        pos += cache[i-5] * 23;
        pos += cache[i-6] * 21;
        pos += cache[i-11] * 17;
        pos += cache[i-12] * 5;

        /* negative contributions */
        neg += cache[i-7] * 25;
        neg += cache[i-8] * 40;
        neg += cache[i-9] * 44;

        /* the result is guaranteed to be non‑negative */
        cache[i] = pos - neg;
    }

    cache_len = needed;
    return cache[(size_t)n];
}
