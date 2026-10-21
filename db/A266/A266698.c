/*
 * A266698  x-values of solutions to the Diophantine equation x^2 - 7*y^2 = 2.
 * https://oeis.org/A266698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=45, a(n)=16*a(n-1)-a(n-2) for n≥3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)45;

    seq_t prev2 = (seq_t)3;
    seq_t prev1 = (seq_t)45;
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = 16 * prev1 - prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
