/*
 * A266007  Number of n X 3 integer arrays with each element equal to the
 *          number of horizontal and antidiagonal neighbors equal to itself.
 * https://oeis.org/A266007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-3) Initial values: a(1)=2, a(2)=2, a(3)=6
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* array holding computed values */
    static idx_t capacity = 0;    /* largest index allocated (last element) */
    static idx_t last = 0;        /* largest index already computed */

    /* Ensure the array can hold the first three base terms. */
    if (capacity < 3) {
        size_t new_cap = 4;                       /* indices 0..3 inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = 3;
    }

    /* Initialise base values if they have not yet been set. */
    if (last < 1) cache[1] = (seq_t)2;
    if (last < 2) cache[2] = (seq_t)2;
    if (last < 3) cache[3] = (seq_t)6;
    if (last < 3) last = 3;

    /* Grow the array if the requested index lies beyond the current capacity. */
    if ((idx_t)n > capacity) {
        size_t new_cap = (size_t)n + 1;           /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = (idx_t)n;
    }

    /* Compute any terms that are still missing. */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = cache[i-1] + (seq_t)4 * cache[i-3];
        }
        last = n;
    }

    return cache[n];
}
