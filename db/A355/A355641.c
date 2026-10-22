/*
 * A355641  Numbers k that can be written as the sum of 5 divisors of k (not
 *          necessarily distinct).
 * https://oeis.org/A355641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers divisible by at least one of 5, 6, 8, 9, 14 or 21 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t k = 0;          /* current integer we test */
    idx_t count = 0;      /* how many qualifying numbers found so far */

    for (;;) {            /* loop until the nth qualifying number is found */
        ++k;
        /* test divisibility by any of the six listed numbers */
        if ((k % 5 == 0) || (k % 6 == 0) || (k % 8 == 0) ||
            (k % 9 == 0) || (k % 14 == 0) || (k % 21 == 0)) {
            ++count;
            if (count == n)
                return k;   /* nth term found */
        }
    }
}
