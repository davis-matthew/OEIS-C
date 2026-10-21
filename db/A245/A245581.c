/*
 * A245581  a(n) = (5 * (1 + (-1)^(1 + n)) + 2 * n^2) / 4.
 * https://oeis.org/A245581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5 * (1 + (-1)^(1 + n)) + 2 * n^2) / 4
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 0-based offset 0.  For even n: a(n) = n^2/2.
       For odd  n: a(n) = (5 + n^2)/2.  Both divisions are exact. */
    if ((n & 1u) == 0) {
        /* even n */
        return ((seq_t)n * (seq_t)n) / 2;
    } else {
        /* odd n */
        return ((seq_t)5 + (seq_t)n * (seq_t)n) / 2;
    }
}
