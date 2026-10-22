/*
 * A318553  a(1) = 1, a(n) = -5*a(n/3) if n is divisible by 3, a(n) = n -
 *          a(n-1) if n + 1 is divisible by 3, otherwise a(n) = n + a(n-1).
 * https://oeis.org/A318553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1) = 1; for n>1: * if n % 3 == 0: a(n) = -5 * a(n/3) * else if
 *            (n+1) % 3 == 0: a(n) = n - a(n-1) * else: a(n) = n + a(n-1)
 * Range:     i8 n <= 29, i16 n <= 1548, i32 n <= 9957, i64 n <= 9957,
 *            i128 n <= 9957, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t computed_max = 0;

    if (n == 0) return 0;

    if (n > computed_max) {
        size_t needed = n + 1;
        if (needed > capacity) {
            size_t new_cap = capacity ? capacity * 2 : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            capacity = new_cap;
        }
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else if (i % 3 == 0) {
                cache[i] = -5 * cache[i / 3];
            } else if ((i + 1) % 3 == 0) {
                cache[i] = (seq_t)i - cache[i - 1];
            } else {
                cache[i] = (seq_t)i + cache[i - 1];
            }
        }
        computed_max = n;
    }
    return cache[n];
}
