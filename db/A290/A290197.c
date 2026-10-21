/*
 * A290197  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 721", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=101, a(3)=1011, a(n) = (10^n-1)/9 for n>=4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)101;
    if (n == 3) return (seq_t)1011;

    seq_t res = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        res = res * (seq_t)10 + (seq_t)1;
    }
    return res;
}
