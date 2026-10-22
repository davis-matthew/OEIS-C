/*
 * A398755  Number of steps to reach 1 starting from n when the function f(x)
 *          = x/7 if 7 divides x, and x+ceiling(x/7) otherwise is iterated, or
 *          -1 if 1 is never reached.
 * https://oeis.org/A398755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence a(n): number of steps to reach 1 starting from n
 * with f(x) = x/7 if 7|x, otherwise f(x) = x + ceil(x/7).
 * a(n) = number of iterations needed to reach 1, or -1 if never reached.
 */

#include <stddef.h>

#ifndef MAX_STEPS
#define MAX_STEPS 1000000   /* safety limit for iterations */
#endif

seq_t a(idx_t n) {
    seq_t val = (seq_t)n;
    seq_t steps = 0;

    if (val == (seq_t)1) return 0;

    while (steps < (seq_t)MAX_STEPS) {
        if (val % (seq_t)7 == (seq_t)0) {
            val = val / (seq_t)7;
        } else {
            val = val + ((val + (seq_t)6) / (seq_t)7);
        }
        steps++;

        if (val == (seq_t)1)
            return steps;
    }

    /* If iteration limit exceeded, assume 1 not reachable */
    return (seq_t)-1;
}
