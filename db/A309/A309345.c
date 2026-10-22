/*
 * A309345  a(n) is the minimum number of transversals in Latin squares of
 *          order n that have at least 1 transversal.
 * https://oeis.org/A309345
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 3 if n is odd, 8 if n is even (offset 3)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values are defined only for n >= 3, but we return 0 for smaller n */
    if (n < 3) return 0;
    /* n is idx_t (unsigned), use bitwise check for odd/even */
    return ( (n & 1) ? (seq_t)3 : (seq_t)8 );
}
