/*
 * A283707  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 721", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=0; a(2)=111; a(3)=1101; For n>=4: a(n) = decimal
 *            integer consisting of n ones followed by a 0 (i.e. a(n) =
 *            ((10^n-1)/9)*10 )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) {
        seq_t val = 0;
        for (idx_t i = 0; i < 3; ++i) val = val * 10 + (seq_t)1;
        return val;
    }
    if (n == 3) {
        seq_t val = 0;
        val = val * 10 + (seq_t)1;
        val = val * 10 + (seq_t)1;
        val = val * 10 + (seq_t)0;
        val = val * 10 + (seq_t)1;
        return val;
    }
    /* n >= 4: build n ones then a trailing zero */
    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i) {
        val = val * 10 + (seq_t)1;
    }
    val = val * 10;   /* append zero */
    return val;
}
