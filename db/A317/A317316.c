/*
 * A317316  Multiples of 16 and odd numbers interleaved.
 * https://oeis.org/A317316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 16*n, a(2n+1) = 2*n+1 (i.e. a(odd) = n, a(even) =
 *            16*(n/2))
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the index is odd, the value equals the index itself.  */
    if (n & 1U) {
        return (seq_t)n;
    }
    /* If the index is even, compute 16 * (n/2).  */
    seq_t half = (seq_t)(n / 2);
    return half * (seq_t)16;
}
