/*
 * A316319  Coordination sequence for a trivalent node in a chamfered version
 *          of the 3^6 triangular tiling of the plane.
 * https://oeis.org/A316319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n - 21 for n > 5; a(0)=1, a(1)=3, a(2)=7, a(3)=14,
 *            a(4)=25, a(5)=38
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small known values */
    static const seq_t pre[6] = {
        (seq_t)1,
        (seq_t)3,
        (seq_t)7,
        (seq_t)14,
        (seq_t)25,
        (seq_t)38
    };

    if (n < 6) {
        return pre[n];
    }
    /* For n > 5, use the linear formula */
    return ((seq_t)n * (seq_t)12) - (seq_t)21;
}
