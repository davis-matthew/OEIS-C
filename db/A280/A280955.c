/*
 * A280955  Number of n X 2 0..2 arrays with no element equal to more than one
 *          of its horizontal, vertical and antidiagonal neighbors and with
 *          new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A280955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) + a(n-3) for n > 4, a(1) = 2, a(2) =
 *            9, a(3) = 42, a(4) = 205
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_max = 0;   /* largest n for which cache[n-1] is valid */

    /* ensure at least idea to hold n */
    if (cache_size < n) {
        idx_t new_size = cache_size ? cache_size * 2 : 4;
        if (new_size < n) new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* out of memory – abort */
            return 0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* initialize first four terms if not yet done */
    if (computed_max < 4) {
        /* make sure indices up to 4 are valid */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)42;
        cache[3] = (seq_t)205;
        computed_max = (cache_size < 4) ? cache_size : 4;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed_max; i < n; ++i) {
        /* recurrence: a(i+1) = 4*a(i) + 4*a(i-1) + a(i-2) */
        seq_t term = (seq_t)4 * cache[i-1] + (seq_t)4 * cache[i-2] + cache[i-3];
        cache[i] = term;
    }
    computed_max = n;

    return cache[n-1];
}
