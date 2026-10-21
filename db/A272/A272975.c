/*
 * A272975  Numbers that are congruent to {0,7} mod 12.
 * https://oeis.org/A272975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n - 11 + (-1)^n)/2 for n >= 1
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Ensure n >= 1 as per specification. */
    seq_t nn = (seq_t)n;          /* cast index to the arithmetic type */
    seq_t term;
    /* (-1)^n is +1 if n even, -1 if n odd */
    if ((n & 1U) == 0U) {
        /* n is even: (12*n - 11 + 1)/2 = (12*n - 10)/2 = 6*n - 5 */
        term = 6 * nn - 5;
    } else {
        /* n is odd: (12*n - 11 - 1)/2 = (12*n - 12)/2 = 6*(n-1) */
        term = 6 * (nn - 1);
    }
    return term;
}
