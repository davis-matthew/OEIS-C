/*
 * A245341  Sum of digits of n written in fractional base 5/2.
 * https://oeis.org/A245341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(A024632(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      a(n) = sum of the digits in the unique representation of n
 *      in fractional base 5/2.                                   */

seq_t a(idx_t n)
{
    seq_t sum = 0;          /* sum of digits (seq_t accumulator) */
    idx_t cur = n;          /* working copy of n (idx_t) */

    while (cur > 0) {
        seq_t d = cur % 5;              /* least significant digit (seq_t) */
        sum += d;                       /* add digit to sum */

        /* Convert digit back to idx_t for subsequent arithmetic.
         * d is always < 5, so this cast is safe. */
        idx_t r = (idx_t)d;

        /* Next term: (cur - d) is divisible by 5.
         *   cur' = (cur - d)/5 * 2
         *   The order prevents overflow for all idx_t values. */
        cur = ((cur - r) / 5) * 2;
    }

    return sum;      /* total digit sum */
}
