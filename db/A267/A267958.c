/*
 * A267958  4 times A042965.
 * https://oeis.org/A267958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267958: differences between even squares.
   Recurrence: a(n) = a(n-1) + a(n-3) - a(n-4) for n > 4,
   with initial values a(1)=0, a(2)=4, a(3)=12, a(4)=16. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* capacity of the cache (elements, 1‑based) */
    static size_t last = 0; /* largest n for which cache[n] has been set */

    if (n == 0)   /* below the offset; behavior not important */
        return (seq_t)0;

    /* Grow the cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;   /* we use 1‑based indexing */
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        } else {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        }
        cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)0;
        } else if (i == 2) {
            cache[2] = (seq_t)4;
        } else if (i == 3) {
            cache[3] = (seq_t)12;
        } else if (i == 4) {
            cache[4] = (seq_t)16;
        } else {
            cache[i] = cache[i - 1] + cache[i - 3] - cache[i - 4];
        }
    }
    last = (size_t)n;
    return cache[n];
}
