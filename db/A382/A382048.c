/*
 * A382048  Starting from n and decrement, d = 1 we repeatedly subtract d
 *          until we reach a multiple of d+1. Whereupon we set d := d+1 and
 *          continue the process. a(n) is the total number of subtractions
 *          required to reduce n to 0.
 * https://oeis.org/A382048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of subtractions performed by the process
 *            defined in OEIS A382048
 * Range:     u8 n <= 845, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Starting with x = n and d = 1, repeatedly subtract d from x until the remainder is
   a multiple of d+1. Then increment d and continue.  The function counts how many
   subtractions are made until x reaches 0. */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;   /* current value being reduced */
    seq_t d = 1;          /* current subtraction step size */
    seq_t count = 0;      /* total number of subtractions performed */

    while (x > 0) {
        x -= d;           /* perform one subtraction */
        count++;          /* increment step counter */

        /* If the current value x is a multiple of d+1, increase the step size. */
        if ((x % (d + 1)) == 0) {
            d++;
        }
    }
    return count;
}
