/*
 * A270302  Numbers which are representable as a sum of seventeen but no fewer
 *          consecutive nonnegative integers.
 * https://oeis.org/A270302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 272 * m where m is the nth positive integer not divisible
 *            by any of 3, 5, 7, 11, 13. The sequence starts at n=1
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, guard */
    idx_t count = 0;                      /* how many valid m found */
    idx_t m = 1;                           /* candidate m */

    while (1) {
        /* reject m divisible by any of the primes 3,5,7,11,13 */
        if (!(m % 3 == 0 ||
              m % 5 == 0 ||
              m % 7 == 0 ||
              m % 11 == 0 ||
              m % 13 == 0)) {
            ++count;
            if (count == n) {
                /* true multiplication in the target type */
                return (seq_t)272 * (seq_t)m;
            }
        }
        ++m;
    }
}
