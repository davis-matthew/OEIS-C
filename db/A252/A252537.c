/*
 * A252537  Number of (5+2) X (n+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-3) - 32*a(n-6) for n >= 10 Initial values:
 *            a(1)=546, a(2)=676, a(3)=1114, a(4)=1676, a(5)=4420, a(6)=7184,
 *            a(7)=11456, a(8)=31136, a(9)=50560
 * Range:     u8 none, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[9] = {
    (seq_t)546, (seq_t)676, (seq_t)1114, (seq_t)1676,
    (seq_t)4420, (seq_t)7184, (seq_t)11456, (seq_t)31136,
    (seq_t)50560
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */
    if (n <= 9) return init[n - 1];

    if (cache == NULL || cache_size < n) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failed; in a contest environment would abort.
               Here we simply return 0 to avoid undefined behaviour. */
            return (seq_t)0;
        }
        cache = new_cache;

        /* If cache was newly allocated or expanded, fill initial terms */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 9) {
                cache[i] = init[i];
            } else {
                /* a(i+1) = 12*a(i-2) - 32*a(i-5)  (indices are zero-based) */
                cache[i] = (12 * cache[i - 3]) - (32 * cache[i - 6]);
            }
        }
        cache_size = new_size;
    }

    return cache[n - 1];
}
