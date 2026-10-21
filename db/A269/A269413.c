/*
 * A269413  Number of length-7 0..n arrays with no repeated value greater than
 *          or equal to the previous repeated value.
 * https://oeis.org/A269413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) +
 *          56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8)   for n > 8
 * Initial values:
 *   a(1) = 20,  a(2) = 957,  a(3) = 10132,  a(4) = 56890,
 *   a(5) = 223320, a(6) = 695135, a(7) = 1837752, a(8) = 4302612
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for index 0; return 0 as a safe value */
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* currently allocated capacity      */
    static size_t   len   = 0;   /* number of already computed items */

    size_t needed = (size_t)n;
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)   /* allocation failed – unlikely */
            return (seq_t)0;
        cache = tmp;
        cap   = needed;
    }

    /* initial values (first eight terms) */
    static const seq_t init_vals[8] = {
        (seq_t)20,
        (seq_t)957,
        (seq_t)10132,
        (seq_t)56890,
        (seq_t)223320,
        (seq_t)695135,
        (seq_t)1837752,
        (seq_t)4302612
    };

    if (len < needed) {
        size_t i;

        /* fill the first eight terms if needed */
        for (i = len; i < needed && i < 8; ++i)
            cache[i] = init_vals[i];

        /* compute remaining terms using the recurrence */
        for ( ; i < needed; ++i) {
            cache[i] =
                ( 8 * cache[i-1]
                 - 28 * cache[i-2]
                 + 56 * cache[i-3]
                 - 70 * cache[i-4]
                 + 56 * cache[i-5]
                 - 28 * cache[i-6]
                 +  8 * cache[i-7]
                 -   cache[i-8] );
        }
        len = needed;
    }

    return cache[n-1];
}
