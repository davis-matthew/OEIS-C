/*
 * A258021  Eventual fixed point of map x -> floor(tan(x)) when starting the
 *          iteration with the initial value x = n.
 * https://oeis.org/A258021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n == floor(tan(n)), else a(n) = a(floor(tan(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses an iterative evaluation of tan(t) with long double. */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Intermediate values are kept in a signed 64‑bit integer because
     * the iteration may visit negative numbers.  The argument range
     * (n ≤ 10 000) guarantees that all intermediate values stay within
     * the bounds of int64_t and the final result fits in seq_t. */
    int64_t x = (int64_t)n;

    while (1) {
        long double t  = tanl((long double)x);
        int64_t   y    = (int64_t)floorl(t);

        if (x == y)
            return (seq_t)x;

        x = y;
    }
}
