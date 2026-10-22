/*
 * A309336  a(n) = n^4 if n odd, 15*n^4/16 if n even.
 * https://oeis.org/A309336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^4 if n odd * 15*n^4/16 if n even * (equivalently a(n) =
 *            n^4 * (31 - (-1)^n)/32)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t, then compute n^4. */
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;
    seq_t n4   = n2 * n2;

    /* If n is odd, return n^4; otherwise return 15*n^4/16. */
    if ((n & 1) != 0) {
        return n4;
    } else {
        return (n4 * 15) / 16;
    }
}
