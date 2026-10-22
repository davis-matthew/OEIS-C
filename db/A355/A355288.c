/*
 * A355288  a(0)=1, a(1)=3, a(2)=7; thereafter a(n) = a(n-1) + a(n-2) + 1.
 * https://oeis.org/A355288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=7; thereafter a(n)=a(n-1)+a(n-2)+1 for n>2
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;

    seq_t prev2 = (seq_t)3; /* a(n-2) */
    seq_t prev1 = (seq_t)7; /* a(n-1) */
    seq_t val;

    for (idx_t i = 3; i <= n; ++i) {
        val = prev1 + prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = val;
    }
    return prev1;
}
