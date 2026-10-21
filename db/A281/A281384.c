/*
 * A281384  Least integer with more than 1 digit in base n, such that the set
 *          of its base-n digits equals the set of its binary digits.
 * https://oeis.org/A281384
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if (n + 1) is not a power of two, otherwise a(n) = n^2
 * Range:     u8 n <= 30, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 2. For smaller indices return 0. */
    if (n < 2) {
        return (seq_t)0;
    }
    idx_t x = n + 1;                /* x = n + 1 */
    /* Check whether x is a power of two: only one bit set */
    if ((x & (x - 1)) == 0) {
        return (seq_t)n * (seq_t)n; /* n^2 will fit existing seq_t where the
                                       ground truth fits that type            */
    } else {
        return (seq_t)n;
    }
}
