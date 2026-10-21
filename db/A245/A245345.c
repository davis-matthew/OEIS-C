/*
 * A245345  Sum of digits of n written in fractional base 9/2.
 * https://oeis.org/A245345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(A024650(n)) The integer n is expressed in base
 *            9/2. Its least–significant digit is n mod 9. Removing that digit
 *            leaves m = (n - (n mod 9)) * 2 / 9 = 2 * floor(n / 9). Thus a(n)
 *            = (n mod 9) + a( 2 * floor(n/9) ), with a(0) = 0. This
 *            recurrence is implemented below. It runs in O(log₉...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for standard integer types, if needed */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n != 0) {
        idx_t r = n % 9;          /* least significant digit in base 9/2 */
        sum += (seq_t)r;          /* add to digit sum */
        n = (n / 9) * 2;          /* move to next higher digit position */
    }
    return sum;
}
