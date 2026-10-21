/*
 * A255591  Convert n to base 6, move least significant digit to most
 *          significant digit and convert back to base 10.
 * https://oeis.org/A255591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n % 6) * 6^(number_of_base6_digits(n)-1) + floor(n / 6)
 * Range:     u8 n <= 217, u16 n <= 9504, u32 n <= 9504, u64 n <= 9504,
 *            u128 n <= 9504, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 6 the rotation does not change the number */
    if (n < 6) {
        return (seq_t)n;
    }

    /* Determine k = number_of_digits(n) - 1
       i.e., the exponent for the 6 in the rotated position */
    idx_t temp = n;
    idx_t k = 0;
    while (temp >= 6) {
        temp /= 6;
        ++k;
    }

    /* Compute 6^k */
    seq_t pow = 1;
    for (idx_t i = 0; i < k; ++i) {
        pow *= 6;
    }

    /* Least significant digit and the remaining high part */
    seq_t d    = (seq_t)(n % 6);
    seq_t high = (seq_t)(n / 6);

    return d * pow + high;
}
