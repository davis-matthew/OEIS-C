/*
 * A257285  a(n) = 4*5^n - 3*4^n.
 * https://oeis.org/A257285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*5^n - 3*4^n Recurrence: a(0) = 1, a(1) = 8, a(n) =
 *            9*a(n-1) - 20*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)8;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)9 * prev1 - (seq_t)20 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
