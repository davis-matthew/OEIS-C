/*
 * A373912  Number of compositions of 7*n into parts 6 and 7.
 * https://oeis.org/A373912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) * +
 *            21*a(n-5) - 6*a(n-6) + a(n-7) * Initial values: a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=1, a(4)=1, a(5)=1, a(6)=2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Ensure cache is big enough for n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = alloc_seq_array(new_cap);

        /* Copy existing values if any */
        if (cache != NULL) {
            for (size_t i = 0; i < cap; ++i) {
                new_cache[i] = cache[i];
            }
            free_seq_array(cache);
        }
        cache = new_cache;
        cap = new_cap;

        /* Initialize base values if we haven't yet or if we expanded from 0 */
        if (cap >= 1) { cache[0] = 1; }
        if (cap >= 2) { cache[1] = 1; }
        if (cap >= 3) { cache[2] = 1; }
        if (cap >= 4) { cache[3] = 1; }
        if (cap >= 5) { cache[4] = 1; }
        if (cap >= 6) { cache[5] = 1; }
        if (cap >= 7) { cache[6] = 2; }

        /* Compute values that are not yet in cache */
        for (size_t i = 7; i < cap; ++i) {
            cache[i] = 7*cache[i-1]
                      - 21*cache[i-2]
                      + 35*cache[i-3]
                      - 35*cache[i-4]
                      + 21*cache[i-5]
                      - 6*cache[i-6]
                      + cache[i-7];
        }
    }

    return cache[n];
}
