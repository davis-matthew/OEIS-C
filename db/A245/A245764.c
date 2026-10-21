/*
 * A245764  a(n) = 2*(n^2 + 1) + n*(1 + (-1)^n).
 * https://oeis.org/A245764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n^2 + 1) + n*(1 + (-1)^n)
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert the index to the sequence type first */
    seq_t sn = (seq_t)n;

    /* Base part: 2*(n^2 + 1) */
    seq_t result = (seq_t)2 * (sn * sn + (seq_t)1);

    /* Add n*(1 + (-1)^n).  If n is even, (-1)^n = 1 → term = 2*n,
       if n is odd, (-1)^n = -1 → term = 0. */
    if ((n & 1) == 0) {
        result += (seq_t)2 * sn;
    }

    return result;
}
