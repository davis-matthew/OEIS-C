/*
 * A289144  The difference between the second divisor of n and the penultimate
 *          divisor of n.
 * https://oeis.org/A289144
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = spf(n) - n / spf(n) (difference between second and
 *            penultimate divisor)
 * Range:     i8 n <= 130, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                         /* undefined for n < 2, return 0 for safety */

    uint64_t spf = _smallest_prime_factor(n);
    seq_t part1 = (seq_t)spf;              /* second divisor */
    seq_t part2 = (seq_t)(n / spf);        /* penultimate divisor */
    return part1 - part2;                  /* difference */
}
