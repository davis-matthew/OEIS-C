/*
 * A258958  Number of (n+2) X (n+2) 0..1 arrays with no 3 X 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 0 or 3 and
 *          no column sum 0 or 3.
 * https://oeis.org/A258958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)..a(10) are the given initial values. For n>10 the empirical
 *            recurrence a(n)=4*a(n-4) applies
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 51, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Initial values for n = 1 … 10 */
static const seq_t initial[11] = {
    0,            /* unused, index 0 */
    66,  344, 192, 312, 384, 694, 720, 1210, 1440, 2674
};

seq_t a(idx_t n)
{
    /* n ≥ 1.  If n is 0 (should not happen) return 0. */
    if (n == 0u) return (seq_t)0;

    /* Reduce n to the range 1…10 by repeatedly subtracting 4,
       accumulating the factor 4^(number of subtractions). */
    idx_t m = n;
    seq_t factor = (seq_t)1;
    while (m > 10u) {
        m -= 4u;
        factor *= (seq_t)4;
    }

    /* Multiply the known value by the accumulated factor. */
    return initial[m] * factor;
}
