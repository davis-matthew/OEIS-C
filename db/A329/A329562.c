/*
 * A329562  a(n) = 2^(sum of digits of n).
 * https://oeis.org/A329562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2^(sum of decimal digits of n)
 * Range:     u8 n <= 7, u16 n <= 78, u32 n <= 5998, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    unsigned int sum = 0;

    /* Sum the decimal digits of n */
    while (tmp > 0) {
        sum += (unsigned) (tmp % 10);
        tmp /= 10;
    }

    /* Compute 2^sum by repeated left-shifts on seq_t */
    seq_t res = (seq_t)1;
    while (sum--) {
        res <<= 1;
    }

    return res;
}
