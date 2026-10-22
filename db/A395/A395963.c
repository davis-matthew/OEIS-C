/*
 * A395963  Numbers k such that A395961(k) = 1.
 * https://oeis.org/A395963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(n-1)+1 - A395961(2*(n-1)), * where A395961(m) = 1 if m
 *            ≥ 16 and ((m-16)/16) is even, otherwise 0. * This function
 *            returns the nth term of sequence A395963
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)n << 1) - 2;          /* m = 2*(n-1) */
    seq_t A = 0;
    if (m >= 16) {
        seq_t k = (m - 16) >> 4;           /* (m-16)/16 */
        if ((k & 1) == 0)
            A = 1;
    }
    return ((seq_t)n << 1) - 1 - A;         /* 2*n - 1 - A */
}
