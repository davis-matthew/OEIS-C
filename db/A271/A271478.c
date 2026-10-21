/*
 * A271478  If n is even, a(n)=n/2, otherwise 2*n+2.
 * https://oeis.org/A271478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n even, a(n)=n/2; otherwise a(n)=2*n+2
 * Range:     u8 n <= 126, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1U) == 0) {
        return (seq_t)n / 2;
    } else {
        return ((seq_t)n * 2) + 2;
    }
}
