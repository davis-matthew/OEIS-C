/*
 * A273847  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 969", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1+2*n)^2 for n > 4 initial values: a(0)=1, a(1)=4,
 *            a(2)=21, a(3)=45, a(4)=76
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pre[5] = { (seq_t)1, (seq_t)4, (seq_t)21, (seq_t)45, (seq_t)76 };
    if (n < 5) return pre[n];
    seq_t x = (seq_t)n;
    x = x * (seq_t)2 + (seq_t)1;
    return x * x;
}
