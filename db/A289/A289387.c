/*
 * A289387  a(n) = Sum_{k>=0} (-1)^k*binomial(n, 5*k+2).
 * https://oeis.org/A289387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k>=0} (-1)^k * binomial(n, 5*k+2)
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 34, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (for n >= 5): a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) */
/* Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=3, a(4)=6 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cached values up to seq_len-1 */
static idx_t seq_len   = 0;        /* number of computed entries */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n < 5) {
        static seq_t const init[5] = {0, 0, 1, 3, 6};
        return init[n];
    }

    /* Allocate cache on first use and initialize first five terms */
    if (!seq_cache) {
        seq_cache = (seq_t *)malloc(5 * sizeof(seq_t));
        seq_cache[0] = 0;
        seq_cache[1] = 0;
        seq_cache[2] = 1;
        seq_cache[3] = 3;
        seq_cache[4] = 6;
        seq_len   = 5;
    }

    /* Extend the cache up to n if necessary */
    if (n >= seq_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; abort with a reasonable error code */
            return 0;
        }
        seq_cache = tmp;

        /* Compute terms from the first missing index up to n */
        for (idx_t i = seq_len; i <= n; ++i) {
            seq_cache[i] = 5 * seq_cache[i-1]
                         -10 * seq_cache[i-2]
                         +10 * seq_cache[i-3]
                         - 5 * seq_cache[i-4];
        }
        seq_len = new_len;
    }

    return seq_cache[n];
}
