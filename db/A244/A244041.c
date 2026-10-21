/*
 * A244041  Sum of digits of n written in fractional base 4/3.
 * https://oeis.org/A244041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007953(A024631(n))
 * Range:     u8 n <= 298, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value a(n) is the sum of the digits of the unique representation of n
 * in the fractional base in which each digit lives in {0,1,2,3} and the base
 * is 4/3.  The algorithm repeatedly extracts the least‑significant digit as
 * n mod 4, adds it to the sum and then divides n by 4/3, i.e. performs
 * n = (n - digit) * 3 / 4.  This keeps all values integral and fits the
 * restrictions on operations. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t value = (seq_t)n;          /* current value of n in the loop */

    /* Handle n = 0 immediately: the sum of its digits is 0 */
    while (value != 0) {
        seq_t digit = value % (seq_t)4;   /* least significant digit (0..3) */
        sum += digit;
        /* divide value by 4/3: (value - digit) / 4 * 3 */
        value = (value - digit) >> 2;     /* (value - digit) / 4  */
        value *= (seq_t)3;                /* multiply by 3           */
    }
    return sum;
}
