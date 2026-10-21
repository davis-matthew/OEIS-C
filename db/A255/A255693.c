/*
 * A255693  Convert n to base 8, move the most significant digit to the least
 *          significant one and convert back to base 10.
 * https://oeis.org/A255693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   Replace the most significant octal digit of n with the least
 *            significant digit and interpret in base 10. i.e. let pow =
 *            8^⌊log₈ n⌋, msd = n / pow, rest = n % pow, then a(n) = rest * 8
 *            + msd
 * Range:     u8 n <= 95, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;     /* convert index to sequence type */
    seq_t pow = 1;

    /* Find the highest power of 8 <= x without overflow.
       Use division to avoid overflow in multiplication. */
    while (pow <= x / 8) {
        pow *= 8;
    }

    seq_t msd  = x / pow;     /* most significant octal digit */
    seq_t rest = x % pow;     /* remaining digits */

    return rest * 8 + msd;
}
