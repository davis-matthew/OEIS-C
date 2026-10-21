/*
 * A271508  Numbers that are congruent to {1,4} mod 10.
 * https://oeis.org/A271508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - 5 - (-1)^n
 * Range:     u8 n <= 52, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 5*n - 5, then adjust for (-1)^n */
    seq_t val = (seq_t)5 * (seq_t)n - (seq_t)5;
    if (n & 1)         /* n is odd → (-1)^n = -1 → subtract(-1) = +1 */
        val += (seq_t)1;
    else                /* n is even → (-1)^n = +1 → subtract(1) */
        val -= (seq_t)1;
    return val;
}
