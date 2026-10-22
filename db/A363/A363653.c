/*
 * A363653  a(1) = 1; for n > 1, a(n) = a(n-1) - A000005(n) if a(n) strictly
 *          positive, else a(n) = a(n-1) + A000005(n).
 * https://oeis.org/A363653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n-1) - A000005(n) if a(n-1) >
 *            A000005(n), else a(n-1) + A000005(n)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] unused, cache[1] = 1 */
    static idx_t last = 0;        /* largest index currently computed */

    if (n < 1)
        return (seq_t)0;          /* undefined, return 0 */

    /* initialise cache for a(1) */
    if (cache == NULL) {
        /* Allocate space for indices 0..1 */
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        if (cache == NULL)
            return (seq_t)0;      /* malloc failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;     /* a(1) = 1 */
        last   = 1;
    }

    /* expand cache up to n if necessary */
    if (n > last) {
        /* New size includes index 0..n */
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_size + 1));
        if (tmp == NULL)
            return (seq_t)0;      /* realloc failed */
        cache = tmp;

        for (idx_t i = last + 1; i <= new_size; ++i) {
            seq_t d = (seq_t)divisor_count_u64((uint64_t)i);
            if (cache[i - 1] > d)
                cache[i] = cache[i - 1] - d;
            else
                cache[i] = cache[i - 1] + d;
        }
        last = new_size;
    }

    return cache[n];
}
