/*
 * A337751  a(n) = n! * Sum_{k=0..floor(n/4)} (-1)^k / (n-4*k)!.
 * https://oeis.org/A337751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 - n*(n-1)*(n-2)*(n-3)*a(n-4) initial values:
 *            a(0)=a(1)=a(2)=a(3)=1 The recurrence follows from the
 *            definition: a(n) = n! * Sum_{k=0..⌊n/4⌋} (-1)^k / (n-4k)! (OEIS
 *            A337751) The implementation uses memoized dynamic programming
 *            for O(n) time
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n) {
    /* Static cache to store already computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;      /* allocated size of cache          */
    static size_t computed = 0;      /* number of initialized entries    */

    /* Determine how many entries are needed (indices 0..n inclusive) */
    size_t need = (size_t)n + 1;

    /* Resize cache if necessary */
    if (need > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 4;
        if (new_cap < need) new_cap = need;
        seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* If reallocation fails, the program will terminate; the build
           environment is expected to allow this for simplicity. */
        cache = ptr;
        capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    for (size_t i = computed; i < need; ++i) {
        if (i < 4) {
            cache[i] = 1;                 /* initial values */
        } else {
            /* Compute prod = i*(i-1)*(i-2)*(i-3) in seq_t to avoid overflow
               as much as possible when seq_t is a native type. */
            seq_t ii = (seq_t)i;
            seq_t prod = ii * (ii - 1) * (ii - 2) * (ii - 3);
            cache[i] = 1 - prod * cache[i - 4];
        }
    }
    computed = need;                    /* update computed count */

    return cache[(size_t)n];
}
