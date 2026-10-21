/*
 * A290834  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 969", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^(n+1)-1)/9 for n >=5; special values: a(0)=1,
 *            a(1)=10, a(2)=101, a(3)=1111, a(4)=11100
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {1, 10, 101, 1111, 11100, 111111};
    if (n < 6)
        return init[n];
    seq_t res = init[5];
    for (idx_t i = 6; i <= n; ++i)
        res = res * 10 + 1;
    return res;
}
