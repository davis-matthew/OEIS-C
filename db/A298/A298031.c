/*
 * A298031  Coordination sequence of Dual(3.4.6.4) tiling with respect to a
 *          tetravalent node.
 * https://oeis.org/A298031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=10, a(3)=16; for n>=4 a(n) = 9*n-6 if n is
 *            even, otherwise a(n) = 9*n-9
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first few terms */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)4;
    if (n == 2U) return (seq_t)10;
    if (n == 3U) return (seq_t)16;

    /* n >= 4 */
    seq_t tn = (seq_t)n;          /* promote the index to seq_t */
    if ((n & 1ULL) == 0ULL) {     /* even n */
        return ((seq_t)9 * tn) - (seq_t)6;
    } else {                      /* odd n */
        return ((seq_t)9 * tn) - (seq_t)9;
    }
}
