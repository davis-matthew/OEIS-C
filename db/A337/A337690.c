/*
 * A337690  a(n) is the number of primitive nondeficient numbers (A006039)
 *          dividing n.
 * https://oeis.org/A337690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} 1 if d is a primitive nondeficient number. (Count
 *            of primitive nondeficient divisors of n.)
 * Range:     u8 n <= 8326, u16 n <= 8326, u32 n <= 8326, u64 n <= 8326,
 *            u128 n <= 8326, big any n
 * Helpers:   compute_sigma, recompute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_sigma.h"
#include "../utils/recompute.h"

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Static data to persist between calls. */
    static idx_t cur_limit = 0;
    static seq_t *a_arr = NULL;

    /* Recompute if we haven't covered this n yet. */
    if (n > cur_limit) {
        /* Clean up old data if any. */
        if (a_arr) {
            free(a_arr);
            a_arr = NULL;
        }
        /* Compute new arrays up to n. */
        recompute(n, NULL, &a_arr);
        cur_limit = n;
    }

    return a_arr[n];
}
