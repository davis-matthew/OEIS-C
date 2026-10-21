/*
 * A254056  Sum two last digits of the sequence to get next term, starting
 *          with 1,2.
 * https://oeis.org/A254056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1, a(2) = 2; for n ≥ 3, a(n) is the sum of the last two
 *            decimal digits of the concatenated sequence a(1)..a(n-1)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static seq_t last_t1 = 0, last_t2 = 0;

    while (len < (size_t)n) {
        /* Initial seed of the sequence */
        if (len == 0) {
            cache = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
            cap = 2;
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)2;
            len = 2;
            last_t1 = (seq_t)1;
            last_t2 = (seq_t)2;
            if (len >= (size_t)n) break;
        }

        /* Ensure capacity for the next term */
        if (len >= cap) {
            size_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 2;
            if (new_cap < len + 1) new_cap = len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap = new_cap;
        }

        /* Compute the next term */
        seq_t z = last_t1 + last_t2;
        cache[len] = z;

        /* Update the last two digits state */
        if (z < (seq_t)10) {
            last_t1 = last_t2;
            last_t2 = z;
        } else {
            seq_t t = z / (seq_t)10;   /* tens digit */
            seq_t u = z % (seq_t)10;   /* units digit */
            last_t1 = t;
            last_t2 = u;
        }
        ++len;
    }

    return cache[n - 1];
}
