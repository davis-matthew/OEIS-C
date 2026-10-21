/*
 * A257700  Number of steps from n to 1 using this algorithm: x -> floor(r*x)
 *          if x is odd, and x -> floor(x/r) if x is even, where r = 5/3.
 * https://oeis.org/A257700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 2894, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A257700
   a(n) = number of steps to reach 1 using
          x -> floor((5/3)*x) if x is odd,
          x -> floor(x/(5/3)) = floor((3/5)*x) if x is even.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = n;      /* current value */
    seq_t steps = 0;  /* count of steps */
    while (x != (seq_t)1) {
        if ((x & (seq_t)1) != (seq_t)0) {          /* x is odd   */
            x = (seq_t)((seq_t)5 * x) / (seq_t)3; /* floor(5x/3) */
        } else {                                   /* x is even */
            x = (seq_t)((seq_t)3 * x) / (seq_t)5; /* floor(3x/5) */
        }
        steps++;
    }
    return steps;
}
