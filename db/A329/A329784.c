/*
 * A329784  Numbers that are either +-1 (mod 5) or +-22 (mod 55).
 * https://oeis.org/A329784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th positive integer congruent to 1 or 4 mod 5, or
 *            equal to 22 or 33. This implementation builds the values on
 *            demand and memoises them in a static array
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: indices 0 .. size-1 correspond to a(1)..a(size) */
    static seq_t *cache = NULL;
    static idx_t size = 0;          /* number of stored values */
    static size_t capacity = 0;     /* allocated capacity */
    static seq_t last_val = 0;      /* last computed value */

    if (n == 0) return 0;           /* guard against unlikely call with 0 */

    if (n > size) {
        /* enlarge buffer if necessary */
        if (n > capacity) {
            capacity = capacity ? capacity * 2 : 16;
            while (capacity < (size_t)n)
                capacity *= 2;
            cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        }
        /* compute missing terms up to n */
        for (; size < n; ++size) {
            seq_t val = last_val + 1;
            /* find the next valid number */
            while (!((val % 5 == 1) || (val % 5 == 4) ||
                     (val % 55 == 22) || (val % 55 == 33))) {
                ++val;
            }
            cache[size] = val;
            last_val = val;
        }
    }
    return cache[n - 1];
}
