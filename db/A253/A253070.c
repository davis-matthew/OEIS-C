/*
 * A253070  The subsequence A253069(2^n-1).
 * https://oeis.org/A253070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is defined by the linear recurrence a(0) = 1, a(1) = 6,
 *            a(2) = 22, a(3) = 82, a(4) = 302, a(5) = 1106, a(6) = 4066; a(n)
 *            = 3*a(n-1) + 3*a(n-2) - a(n-3) - 6*a(n-4) + 10*a(n-5) - 8*a(n-6)
 *            + 8*a(n-7) for n ≥ 7
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard headers for memory allocation */
#include <stdlib.h>
#include <stddef.h>

/* Compute the n-th term of the sequence A253070 */
seq_t a(idx_t n) {
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values stored   */
    static size_t cache_cap = 0;   /* allocated capacity (bytes) */

    /* Constants used in the recurrence, cast to seq_t to avoid
       unsigned conversion issues. */
    static const seq_t C3  = (seq_t)3;
    static const seq_t C6  = (seq_t)6;
    static const seq_t C8  = (seq_t)8;
    static const seq_t C10 = (seq_t)10;

    /* Lazy initialization of the cache with the first seven terms */
    if (cache_len == 0) {
        cache_cap = 7;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)82;
        cache[4] = (seq_t)302;
        cache[5] = (seq_t)1106;
        cache[6] = (seq_t)4066;
        cache_len = 7;
    }

    /* If the requested index is already cached, return it. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Ensure sufficient capacity to store up to n inclusive. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_cap = needed;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        sum += C3  * cache[i - 1];
        sum += C3  * cache[i - 2];
        sum -=      cache[i - 3];
        sum -= C6  * cache[i - 4];
        sum += C10 * cache[i - 5];
        sum -= C8  * cache[i - 6];
        sum += C8  * cache[i - 7];
        cache[i] = sum;
    }

    /* Update the length of stored values. */
    cache_len = needed;

    return cache[(size_t)n];
}
