/*
 * A349843  Expansion of (1 - x^2)/((1 - x^10)*(1 - x - x^2)).
 * https://oeis.org/A349843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) + a(n-7) + a(n-9) + a(n-10) +
 *            delta(n,0) (with a(0) = 1, a(n<0) = 0)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo      = NULL;   /* allocated array */
    static size_t alloc_size = 0;     /* current allocation size   */
    static size_t computed   = 0;     /* number of entries computed */

    /* Grow the allocation if necessary */
    if ((size_t)n + 1U > alloc_size) {
        size_t new_size = (size_t)n + 1U;
        if (new_size < 10U) new_size = 10U;
        seq_t *new_ptr = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!new_ptr) abort();          /* allocation failure */
        memo = new_ptr;
        alloc_size = new_size;
    }

    /* Compute values that have not yet been computed */
    if ((size_t)n + 1U > computed) {
        for (size_t i = computed; i <= (size_t)n; ++i) {
            if (i == 0U) {
                memo[0] = 1;                /* a(0) = 1 */
            } else {
                seq_t s = 0;
                if (i >= 1U) s += memo[i - 1];
                if (i >= 3U) s += memo[i - 3];
                if (i >= 5U) s += memo[i - 5];
                if (i >= 7U) s += memo[i - 7];
                if (i >= 9U) s += memo[i - 9];
                if (i >=10U) s += memo[i -10];
                memo[i] = s;
            }
        }
        computed = (size_t)n + 1U;
    }

    return memo[(size_t)n];
}
