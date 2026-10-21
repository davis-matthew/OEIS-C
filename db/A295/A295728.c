/*
 * A295728  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 1,
 *          a(1) = -1, a(2) = 1, a(3) = 1.
 * https://oeis.org/A295728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   * a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) * a(0) = 1 *
 *            a(1) = -1 * a(2) = 1 * a(3) = 1 * * The sequence is memoised in
 *            a dynamically sized cache so that * calls with arbitrary n (in
 *            any order) are answered in O(n) time * after the first request.
 *            The cache grows exponentially (doubling) * t...
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 43, i64 n <= 89, i128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t count = 0;          /* number of terms currently cached (0‑based) */
    static size_t capacity = 0;       /* allocated capacity of cache */

    /* handle the first four trivial terms directly – no allocation needed */
    if (n < 4) {
        static const seq_t init[4] = { (seq_t)1, (seq_t)-1,
                                       (seq_t)1, (seq_t)1 };
        return init[(size_t)n];
    }

    /* initialise cache on first use */
    if (cache == NULL) {
        capacity = 4;
        count = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
    }

    /* if the requested index is already cached, just return it */
    if ((size_t)n < count)
        return cache[n];

    /* increase allocation if necessary (doubling strategy) */
    size_t old_count = count;
    while ((size_t)n >= capacity)
        capacity = capacity ? capacity * 2 : 4;

    seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute the missing terms from old_count up to n inclusive */
    for (size_t i = old_count; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + 3*cache[i-2] - 2*cache[i-3] - 2*cache[i-4];
    }

    /* update cached count to reflect the newly computed terms */
    count = (size_t)n + 1;

    return cache[n];
}
