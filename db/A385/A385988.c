/*
 * A385988  a(1) = 1, and for any n > 1, a(n) is the largest k < n such that
 *          a(1) + ... + a(k) is a power of 2.
 * https://oeis.org/A385988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 3196, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A385988  
   a(1) = 1; for n > 1, a(n) is the largest k < n such that
   the cumulative sum S(k) = a(1)+…+a(k) is a power of two.  The
   algorithm maintains S(k) for each k and, when a new term is
   required, tests whether S(k) is a power of two.  If it is,
   the next term is k; otherwise the next term repeats a(k). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    static seq_t *a_arr   = NULL;   /* a values, 1‑based */
    static seq_t *cum_arr = NULL;   /* cumulative sums S(k) = sum_{i=1}^k a(i) */
    static idx_t  computed = 0;     /* largest n that has been computed */
    static size_t cap      = 0;     /* allocated capacity (>= computed) */

    if (n == 0)          /* undefined, guard for safety */
        return (seq_t)0;

    /* Ensure storage large enough for indices up to n. */
    if (n > cap) {
        size_t new_cap = (cap == 0) ? 8 : cap * 2;
        if (new_cap < (size_t)n) new_cap = n;
        a_arr   = (seq_t *)realloc(a_arr,   (new_cap + 1) * sizeof(seq_t));
        cum_arr = (seq_t *)realloc(cum_arr, (new_cap + 1) * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialize first term if not already done. */
    if (computed == 0) {
        a_arr[1]   = (seq_t)1;
        cum_arr[1] = a_arr[1];
        computed   = 1;
    }

    /* Compute missing terms up to n. */
    for (idx_t k = computed; k < n; ++k) {
        if (is_power_of_two(cum_arr[k]))
            a_arr[k + 1] = (seq_t)k;
        else
            a_arr[k + 1] = a_arr[k];

        cum_arr[k + 1] = cum_arr[k] + a_arr[k + 1];
    }

    computed = n;
    return a_arr[n];
}
