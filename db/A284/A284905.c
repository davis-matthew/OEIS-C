/*
 * A284905  Fixed point of the morphism 0 -> 01, 1 -> 1001.
 * https://oeis.org/A284905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of the morphism: 0 → 01, 1 → 1001.
   a(1) = 0, a(2) = 1, a(3) = 1, a(4) = 0, … */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0; /* off‑by‑one, not used */
    }

    /* Static cache holding the morphic word when growing it
       to cover the requested index. Thread safety is not
       required by the test harness. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialise with the single symbol '0' if the cache is empty. */
    if (cache == NULL) {
        cache_len = 1;
#ifdef __cplusplus
        cache = new seq_t[cache_len];
#else
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
#endif
        if (cache == NULL) {
            return (seq_t)0; /* allocation failure – unlikely for small n */
        }
        cache[0] = (seq_t)0;
    }

    const seq_t zero = (seq_t)0;
    const seq_t one  = (seq_t)1;

    /* Grow the cached word until it contains at least 'n' symbols. */
    size_t target = (size_t)n;
    while (cache_len < target) {
        /* Compute length of the word after one substitution. */
        size_t old_len = cache_len;
        size_t new_len = 0;
        for (size_t i = 0; i < old_len; ++i) {
            new_len += (cache[i] == zero) ? 2 : 4;
        }

        /* Allocate the new word. */
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_len];
#else
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
#endif
        if (new_cache == NULL) {
            return (seq_t)0; /* allocation failure */
        }

        /* Apply the morphism to each symbol. */
        size_t pos = 0;
        for (size_t i = 0; i < old_len; ++i) {
            if (cache[i] == zero) {          /* 0 → 01 */
                new_cache[pos++] = zero;
                new_cache[pos++] = one;
            } else {                        /* 1 → 1001 */
                new_cache[pos++] = one;
                new_cache[pos++] = zero;
                new_cache[pos++] = zero;
                new_cache[pos++] = one;
            }
        }

        /* Replace the old buffer. */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[target - 1];
}
