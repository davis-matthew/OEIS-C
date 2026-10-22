/*
 * A341009  Numbers whose sum of even digits and sum of odd digits differ by
 *          8.
 * https://oeis.org/A341009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth positive integer whose sum of even digits and sum
 *            of odd digits differ by exactly 8 (in absolute value)
 * Range:     u8 n <= 19, u16 n <= 3273, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static inline bool is_valid(seq_t x) {
    seq_t sum_even = 0, sum_odd = 0;
    seq_t t = x;
    while (t > 0) {
        seq_t d = t % 10;
        t /= 10;
        if (d & 1)
            sum_odd += d;
        else
            sum_even += d;
    }
    seq_t diff = (sum_even >= sum_odd) ? (sum_even - sum_odd)
                                       : (sum_odd - sum_even);
    return diff == 8;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (n == 0) return 0; /* undefined by offset but safe */

    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Extend cache until it contains n elements */
    for ( ; (size_t)n > cache_len; ++cache_len ) {
        seq_t last = (cache_len == 0) ? 0 : cache[cache_len - 1];
        seq_t cur  = last + 1;

        while (!is_valid(cur)) {
            ++cur;
        }

        /* Ensure capacity */
        if (cache_cap <= cache_len) {
            size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
            while (new_cap <= cache_len) new_cap <<= 1;
            void *tmp = realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failure, abort */ abort(); }
            cache = (seq_t *)tmp;
            cache_cap = new_cap;
        }

        cache[cache_len] = cur;
    }

    return cache[n - 1];
}
