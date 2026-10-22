/*
 * A331319  a(n) = [x^n](x - 2*x^3)/(1 - 2*x*(x + 1))^2.
 * https://oeis.org/A331319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A331319: a(n) = 4*a(n-1) - 8*a(n-3) - 4*a(n-4)   for n > 3
   a(0) = 0, a(1) = 1, a(2) = 4, a(3) = 14
   (This is the minimal linear recurrence derived from the generating
   function.) */

#include <stdlib.h>

/* Cached values of the sequence. */
static seq_t *a_vals = NULL;
/* Highest index that has been computed.  When a_vals is NULL the cache
   is empty. */
static idx_t a_max = 0;

/* Ensure that a_vals contains a values for all indices up to n. */
static void
ensure_cache(idx_t n)
{
    /* If values are already cached, nothing to do. */
    if (a_vals && n <= a_max)
        return;

    /* Allocate or grow the cache to hold indices 0 .. n. */
    if (!a_vals) {
        a_vals = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!a_vals) {
            /* Out of memory – abort the program in a portable way. */
            abort();
        }
    } else {
        a_vals = (seq_t *)realloc(a_vals, (size_t)(n + 1) * sizeof(seq_t));
        if (!a_vals) {
            abort();
        }
    }

    /* Compute values from the first missing index up to n. */
    for (idx_t i = (a_vals && a_max > (idx_t)-1) ? a_max + 1 : 0; i <= n; ++i) {
        if (i == 0) {
            a_vals[i] = 0;
        } else if (i == 1) {
            a_vals[i] = 1;
        } else if (i == 2) {
            a_vals[i] = 4;
        } else if (i == 3) {
            a_vals[i] = 14;
        } else {
            a_vals[i] = (seq_t)4  * a_vals[i - 1]
                       - (seq_t)8  * a_vals[i - 3]
                       - (seq_t)4  * a_vals[i - 4];
        }
    }

    /* Update the highest computed index. */
    a_max = n;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return a_vals[n];
}
