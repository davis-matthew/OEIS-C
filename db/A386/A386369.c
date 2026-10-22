/*
 * A386369  a(1) = 0, and for any n > 1, a(n) is the largest k < n such that
 *          a(1) + ... + a(k) is a square number.
 * https://oeis.org/A386369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 70, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) if the sum of the first n-1 terms is not a perfect square;
   otherwise a(n) = n-1.  a(1) = 0. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static seq_t sum = 0;          /* sum of stored terms */
    static seq_t last = 0;         /* a(previous index) */

    if (n < 1) return 0;          /* undefined for n < 1 */

    /* initialise on first call */
    if (len == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;       /* a(1) = 0 */
        sum = 0;
        last = 0;
        len = 1;
    }

    /* extend cache up to requested index n */
    while (len < (size_t)n) {
        /* check whether the current sum is a perfect square */
        seq_t t = floor_sqrt(sum);
        bool is_square = (t * t == sum);

        /* compute the next term */
        seq_t next = is_square ? (seq_t)(len) : last;

        /* grow storage if necessary */
        if (len == cap) {
            cap <<= 1;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        cache[len] = next;      /* store a(len+1) */
        sum += next;
        last = next;
        len++;
    }

    return cache[(size_t)n - 1];
}
