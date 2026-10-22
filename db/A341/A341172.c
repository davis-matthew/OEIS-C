/*
 * A341172  Numbers that when divided by the sum of their digits leave 5 as
 *          remainder.
 * https://oeis.org/A341172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th positive integer x such that x % digit_sum(x) ==
 *            5
 * Range:     u8 n <= 12, u16 n <= 2236, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static seq_t last = 0;   /* largest integer examined so far   */

    if (n == 0) return 0;    /* undefined for offset 1, harmless */

    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    }

    seq_t x = last + (seq_t)1;   /* start searching from last+1 */
    while (len < n) {
        seq_t s = digit_sum(x);
        if (s && (x % s == (seq_t)5)) {
            if (len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[len++] = x;
        }
        ++x;
    }
    last = x - (seq_t)1;   /* record the last tested value */

    return cache[(size_t)(n - 1)];
}
