/*
 * A290192  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 705", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=10, a(2)=101, a(3)=1101, a(4)=11100, a(5)=111101;
 *            a(n) = 10*a(n-1) + a(n-2) - 10*a(n-3) (n > 5)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)101;
    if (n == 3) return (seq_t)1101;
    if (n == 4) return (seq_t)11100;
    if (n == 5) return (seq_t)111101;

    /* maintain the last three terms a(n-3), a(n-2), a(n-1) */
    seq_t prev3 = (seq_t)1101;   /* a(3) */
    seq_t prev2 = (seq_t)11100;  /* a(4) */
    seq_t prev1 = (seq_t)111101; /* a(5) */

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t curr = 10 * prev1 + prev2 - 10 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
