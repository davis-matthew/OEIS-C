/*
 * A284236  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 833", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=110. For n ≥ 3: a(n) is the decimal number
 *            made of “10” repeated ceil(n/2) times. It can be generated
 *            iteratively as val = 0 repeat ceil(n/2) times: val = val*100 +
 *            10 which uses only integer arithmetic that is safe for any seq_t
 *            size
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)110;
    idx_t k = (n + 1) / 2;    /* ceil(n/2) */
    seq_t val = 0;
    for (idx_t i = 0; i < k; ++i)
        val = val * (seq_t)100 + (seq_t)10;
    return val;
}
