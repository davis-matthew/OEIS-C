/*
 * A301727  Partial sums of A301726.
 * https://oeis.org/A301727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 4*a(n-4) + 4*a(n-5) -
 *            4*a(n-6) + 4*a(n-7) - 4*a(n-8) + 4*a(n-9) - 3*a(n-10) + a(n-11)
 *            Initial values: a(0)=1, a(1)=6, a(2)=17, a(3)=33, a(4)=54,
 *            a(5)=81, a(6)=114, a(7)=152, a(8)=195, a(9)=244, a(10)=298,
 *            a(11)=357
 * Range:     u8 n <= 9, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t used  = 0;

    /* small initial values for indices 0..11 */
    static const seq_t init[12] = {
        1,  6,  17, 33, 54, 81, 114, 152, 195, 244, 298, 357
    };

    if (n < 12) {
        return init[(size_t)n];
    }

    /* grow cache if needed */
    if (cap <= (size_t)n) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – return zero as safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise cache with the first 12 terms if not already done */
    if (used <= 11) {
        for (size_t i = used; i < 12; ++i) {
            cache[i] = init[i];
        }
        used = 12;
    }

    /* compute terms from 'used' up to 'n' */
    for (size_t i = used; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)3 * cache[i - 1]
                  - (seq_t)4 * cache[i - 2]
                  + (seq_t)4 * cache[i - 3]
                  - (seq_t)4 * cache[i - 4]
                  + (seq_t)4 * cache[i - 5]
                  - (seq_t)4 * cache[i - 6]
                  + (seq_t)4 * cache[i - 7]
                  - (seq_t)4 * cache[i - 8]
                  + (seq_t)4 * cache[i - 9]
                  - (seq_t)3 * cache[i - 10]
                  + (seq_t)1 * cache[i - 11];
        cache[i] = sum;
    }
    used = (size_t)n + 1;

    return cache[(size_t)n];
}
