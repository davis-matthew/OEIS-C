/*
 * A303005  Number of dominating sets in the n-pan graph.
 * https://oeis.org/A303005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) Initial values: a(1) = 3, a(2) =
 *            5, a(3) = 11
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;
    seq_t prev3 = (seq_t)3;   // a(1)
    seq_t prev2 = (seq_t)5;   // a(2)
    seq_t prev1 = (seq_t)11;  // a(3)
    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = prev1 + prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
