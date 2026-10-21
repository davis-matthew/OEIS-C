/*
 * A298017  Partial sums of A298014.
 * https://oeis.org/A298017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+2*a(n-3)-2*a(n-4)-a(n-6)+a(n-7) for n>10;
   a(0)=1, a(1)=4, a(2)=13, a(3)=28, a(4)=46, a(5)=73, a(6)=110,
   a(7)=147, a(8)=191, a(9)=248, a(10)=302 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t base[11] = {
        (seq_t)1, (seq_t)4, (seq_t)13, (seq_t)28, (seq_t)46,
        (seq_t)73, (seq_t)110, (seq_t)147, (seq_t)191, (seq_t)248, (seq_t)302
    };
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n <= 10) return base[n];

    if (n + 1U > cache_size) {
        idx_t new_size = n + 1U;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        if (cache_size == 0U) {
            /* initialise the base terms */
            for (idx_t i = 0; i <= 10; ++i) new_cache[i] = base[i];
        }
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i <= 10U) continue;          /* base already set */
            seq_t val = new_cache[i - 1]
                        + (seq_t)2 * new_cache[i - 3]
                        - (seq_t)2 * new_cache[i - 4]
                        - new_cache[i - 6]
                        + new_cache[i - 7];
            new_cache[i] = val;
        }
        cache = new_cache;
        cache_size = new_size;
    }
    return cache[n];
}
