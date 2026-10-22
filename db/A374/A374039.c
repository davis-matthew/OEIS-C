/*
 * A374039  a(n) = 1 if the 3-adic valuation of n is not 1, otherwise 0.
 * https://oeis.org/A374039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 if the 3-adic valuation of n is not 1, otherwise 0.
 *            Equivalently, a(n) = 1 if n is not divisible by 3 or if n is
 *            divisible by 9. a(n) = 1 if (n % 3 != 0) || (n % 9 == 0);
 *            otherwise a(n) = 0
 * Range:     u8 n <= 9391, u16 n <= 9391, u32 n <= 9391, u64 n <= 9391,
 *            u128 n <= 9391, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 by OEIS offset. */
    if (n % 3 != 0) {
        return (seq_t)1;
    }
    return (n % 9 == 0) ? (seq_t)1 : (seq_t)0;
}
