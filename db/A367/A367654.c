/*
 * A367654  G.f. A(x) satisfies A(x) = 1 / (1 - x * (1 + x + x^2 + x^3 + x^4)
 *          * A(x^5)).
 * https://oeis.org/A367654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=sum_{k=0}^{n-1} a(floor(k/5)) * a(n-1-k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

static seq_t *cache = NULL;      /* memoised values */
static size_t cache_cap = 0;     /* current allocated capacity */

static void ensure_cache(idx_t n) {
    size_t required = (size_t)n + 1;
    if (cache_cap < required) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < required) new_cap <<= 1;
        size_t old_cap = cache_cap;
        seq_t *tmp;
#ifdef __cplusplus
        tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) abort();
        /* Zero the new portion of the array */
        memset(tmp + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    /* Compute any missing terms up to n */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        if (cache[i] == 0) {
            if (i == 0)
                cache[0] = 1;
            else {
                seq_t sum = 0;
                for (size_t k = 0; k < i; ++k)
                    sum += cache[i - 1 - k] * cache[k / 5];
                cache[i] = sum;
            }
        }
    }
    return cache[(size_t)n];
}
