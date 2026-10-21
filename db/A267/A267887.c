/*
 * A267887  Binary representation of the n-th iteration of the "Rule 237"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=10, a(n)= (10^(2n+1)-1)/9 for n>=2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1u;
    if (n == 1u) return (seq_t)10u;
    /* n >= 2 : construct a decimal string of (2n+1) ones */
    idx_t len = 2 * n + 1;
    seq_t res = (seq_t)0u;
    for (idx_t i = 0; i < len; ++i)
    {
        res = res * (seq_t)10u + (seq_t)1u;
    }
    return res;
}
