/*
 * A303724  Number of nX5 0..1 arrays with every element unequal to 0, 1, 2 or
 *          3 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 35, u64 n <= 74, u128 n <= 152,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A303724:  a(n) = 2*a(n-1) - a(n-4) - a(n-6) - a(n-7) + 2*a(n-8)  (n>12)
    Initial values:
        a(1)=16,   a(2)=120,  a(3)=77,   a(4)=133,
        a(5)=219,  a(6)=345,  a(7)=564,  a(8)=974,
        a(9)=1715, a(10)=2992, a(11)=5222, a(12)=9175
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t max_computed = 0;

    /* grow cache if necessary (need storage up to index n) */
    if (cache == NULL || (size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 64;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (new_cache == NULL) abort();          /* allocation failure */

        /* copy old values if any */
        if (cache) {
            for (size_t i = 0; i < cap; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* initialise base values once */
    if (max_computed == 0) {
        /* ensure capacity for the first 12 terms */
        if (cap < 13) {
            seq_t *tmp = alloc_seq_array(13);
            if (tmp == NULL) abort();
            for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
            free_seq_array(cache);
            cache = tmp;
            cap   = 13;
        }
        cache[0] = (seq_t)0; /* unused */
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)120;
        cache[3] = (seq_t)77;
        cache[4] = (seq_t)133;
        cache[5] = (seq_t)219;
        cache[6] = (seq_t)345;
        cache[7] = (seq_t)564;
        cache[8] = (seq_t)974;
        cache[9] = (seq_t)1715;
        cache[10] = (seq_t)2992;
        cache[11] = (seq_t)5222;
        cache[12] = (seq_t)9175;
        max_computed = 12;
    }

    /* return cached value if already computed */
    if (n <= max_computed) return cache[n];

    /* compute new values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val += cache[i - 1];          /* 2*a(i-1) */
        val -= cache[i - 4];
        val -= cache[i - 6];
        val -= cache[i - 7];
        seq_t twice = cache[i - 8];
        twice += cache[i - 8];         /* 2*a(i-8) */
        val += twice;
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
