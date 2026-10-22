/*
 * A374277  Numbers k divisible by exactly one of the prime factors of 30.
 * https://oeis.org/A374277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers k such that exactly one of 2,3,5 divides k
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t current_max_index = 0;   /* number of terms stored */
    static seq_t last_checked = 0;        /* last integer examined */

    if (n == 0) return (seq_t)0; /* out of bounds, but not used */

    /* Extend cache if needed */
    if (n > current_max_index) {
        /* Ensure cache capacity */
        if (cap == 0) {
            cap = 16;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (cache == NULL) exit(1);
        }
        while (current_max_index < n) {
            seq_t cand = last_checked + 1;
            /* count how many of 2,3,5 divide cand */
            idx_t cnt = ((cand & 1) == 0) + (cand % 3 == 0) + (cand % 5 == 0);
            if (cnt == 1) {
                if (current_max_index == cap) {   /* grow cache */
                    size_t ncap = cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, ncap * sizeof(seq_t));
                    if (tmp == NULL) exit(1);
                    cache = tmp;
                    cap = ncap;
                }
                cache[current_max_index++] = cand;
            }
            last_checked = cand;
        }
    }
    return cache[n - 1];
}
