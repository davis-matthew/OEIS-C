/*
 * A267797  Lucas numbers of the form (x^3 + y^3) / 2 where x and y are
 *          distinct positive integers.
 * https://oeis.org/A267797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - a(n-2), a(1)=76, a(2)=1364
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* off‑offset behaviour */
    if (n == 1) return (seq_t)76;
    if (n == 2) return (seq_t)1364;

    seq_t prev2 = (seq_t)76;       /* a(1) */
    seq_t prev1 = (seq_t)1364;     /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t curr = (seq_t)18 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
