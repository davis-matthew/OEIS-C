/*
 * A278759  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 73", based on the 5-celled von
 *          Neumann neighborhood.
 * https://oeis.org/A278759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) - 4*a(n-4) for n > 7 Initial values: a(0)=1,
 *            a(1)=0, a(2)=3, a(3)=10, a(4)=0, a(5)=63, a(6)=0, a(7)=255
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial segment of the sequence */
    static const seq_t base[8] = {
        (seq_t)1, (seq_t)0, (seq_t)3,  (seq_t)10,
        (seq_t)0, (seq_t)63, (seq_t)0,  (seq_t)255
    };

    if (n < 8)
        return base[n];

    /* circular buffer holding the last 8 values */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = base[i];

    /* compute terms up to n */
    for (idx_t i = 8; i <= n; ++i) {
        buf[i & 7] = (seq_t)5 * buf[(i-2) & 7] - (seq_t)4 * buf[(i-4) & 7];
    }

    return buf[n & 7];
}
