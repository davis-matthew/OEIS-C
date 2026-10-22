/*
 * A307274  Partial sums of A307204.
 * https://oeis.org/A307274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 4, a(2) = 10, a(3) = 16, a(4) = 28, a(5) = 46,
   a(6) = 70, a(7) = 100, a(8) = 130, a(9) = 163, a(10) = 211,
   a(11) = 268, a(12) = 328, a(13) = 388, a(14) = 460, a(15) = 544,
   a(16) = 622, a(17) = 703. For n>17: a(n) = a(n-1) + 2*a(n-4) - 2*a(n-5)
   - a(n-8) + a(n-9).  */

#include <stdlib.h>

static const seq_t base[18] = {
    1, 4, 10, 16, 28, 46, 70, 100, 130, 163,
    211, 268, 328, 388, 460, 544, 622, 703
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t next_to_compute = 18;   /* first index not yet computed */

    /* Return pre‑computed value for small indices */
    if (n < 18) {
        if (cache == NULL) {
            return base[(size_t)n];
        }
        return cache[(size_t)n];
    }

    /* Allocate cache for the first 18 terms if needed */
    if (cache == NULL) {
        capacity = 18;
        cache = (seq_t *)calloc(capacity, sizeof(seq_t));
        for (size_t i = 0; i < 18; ++i) {
            cache[i] = base[i];
        }
    }

    /* Grow the cache if required */
    size_t target = (size_t)n;
    if (target + 1 > capacity) {
        size_t new_cap = capacity * 2;
        if (new_cap < target + 1) new_cap = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute any missing terms using the recurrence */
    for (size_t i = next_to_compute; i <= target; ++i) {
        cache[i] = cache[i - 1]
                 + (cache[i - 4] << 1)
                 - (cache[i - 5] << 1)
                 - cache[i - 8]
                 + cache[i - 9];
    }
    next_to_compute = target + 1;

    return cache[target];
}
