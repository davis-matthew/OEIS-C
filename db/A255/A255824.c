/*
 * A255824  a(n) = n for n < 4; a(4n) = a(n); if every 4th term (a(4), a(8),
 *          a(12), ...) is deleted, this gives back the original sequence.
 * https://oeis.org/A255824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n/4) if n == 0 (mod 4); otherwise a(n) = a(n -
 *            floor(n/4)). Initial values: a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Iterate until n is < 4, then return n.  */
    while (n >= 4U) {
        idx_t q = n / 4U;
        if ((n & 3U) == 0U) {      /* n % 4 == 0  */
            n = q;
        } else {                   /* n % 4 != 0  */
            n -= q;
        }
    }
    return (seq_t)n;
}
