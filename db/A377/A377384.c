/*
 * A377384  a(n) is the number of iterations that n requires to reach a
 *          noninteger or a factorial number under the map x -> x / f(x),
 *          where f(k) = A034968(k) is the sum of digits in the factorial-base
 *          representation of k; a(n) = 0 if n is a factorial number.
 * https://oeis.org/A377384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 634, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorial_base_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of iterations of the map x -> x / f(x) until
   a non-integer or a factorial number is reached, where
   f(x) is the sum of digits in the factorial-base representation
   of x.  a(n)=0 iff n is a factorial number. */

#include <stdbool.h>
#include "../utils/factorial_base_digit_sum.h"

seq_t a(idx_t n)
{
    seq_t current = (seq_t)n;
    seq_t count   = 0;

    while (1) {
        seq_t sum = factorial_base_digit_sum(current);

        /* factorial numbers have digit sum 1 */
        if (sum == 1)
            return count;          /* reached a factorial number */

        ++count;                  /* this iteration is performed */

        /* check whether division yields a non-integer */
        if (current % sum != 0)
            return count;          /* reaches a non-integer now */

        current = current / sum;   /* proceed to the next integer */
    }
}
