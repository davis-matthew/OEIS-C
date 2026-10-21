/*
 * A244151  0-additive sequence: start with a(1) = 2; thereafter, a(n) =
 *          smallest number not already in sequence which is not the sum of
 *          any previous two terms.
 * https://oeis.org/A244151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) for n>6; a(1)=2, a(2)=3, a(3)=4,
 *            a(4)=8, a(5)=9, a(6)=14
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;
    static idx_t   length = 0;   /* number of terms stored */
    static size_t  capacity = 0;

    /* initial terms */
    if (cache == NULL) {
        capacity = 6;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;   /* unlikely in a contest environment */
        cache[0] = 2; cache[1] = 3; cache[2] = 4;
        cache[3] = 8; cache[4] = 9; cache[5] = 14;
        length = 6;
    }

    /* extend cache up to n if necessary */
    if (n > length) {
        if ((size_t)capacity < (size_t)n) {
            size_t new_cap = capacity ? capacity : 1;
            while (new_cap < (size_t)n)
                new_cap <<= 1;                     /* double until large enough */
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                   /* allocation failed */
            cache = tmp;
            capacity = new_cap;
        }
        for (idx_t i = length; i < n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2] - cache[i - 3];
        }
        length = n;
    }

    return cache[n - 1];
}
