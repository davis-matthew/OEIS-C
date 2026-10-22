/*
 * A305865  a(1) = 1, a(n) = -5*a(n/3) if n is divisible by 3, otherwise a(n)
 *          = n - a(n-1).
 * https://oeis.org/A305865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1) = 1 * Recurrence for n > 1: * if n divisible by 3: a(n) =
 *            -5 * a(n/3) * else: a(n) = n - a(n-1)
 * Range:     i8 n <= 27, i16 n <= 984, i32 n <= 9418, i64 n <= 9418,
 *            i128 n <= 9418, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_cached = 0;

    if (n == 0) return 0; /* guard for out-of-range index */

    if (cache == NULL) {
        cap = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_cached = 1;
    }

    if (n > max_cached) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;

        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i % 3 == 0) {
                cache[i] = (seq_t)(-5) * cache[i / 3];
            } else {
                cache[i] = (seq_t)i - cache[i - 1];
            }
        }
        max_cached = n;
    }

    return cache[n];
}
