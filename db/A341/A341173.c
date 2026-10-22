/*
 * A341173  Numbers that when divided by the sum of their digits leave 6 as
 *          remainder.
 * https://oeis.org/A341173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4595, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find numbers n such that n % sum_of_digits(n) == 6.
   a(1) = 34, a(2) = 46, a(3) = 58, …               */
#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t current = 1;           /* next integer to test */
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    if (len < n) {
        /* enlarge cache if necessary */
        if (len == cap) {
            cap *= 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        /* search until we have n terms */
        while (len < n) {
            seq_t s = digit_sum(current);
            if (current % s == (seq_t)6) {
                cache[len++] = current;
                if (len == cap) {
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
            }
            ++current;
        }
    }

    return cache[n - 1];
}
