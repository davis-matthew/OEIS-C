/*
 * A351046  a(1)=1; a(2)=4; for n>2, a(n) = a(n-1) + A000217(n)*a(n-2).
 * https://oeis.org/A351046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + n*(n+1)/2 * a(n-2), a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    seq_t prev2 = 1;   /* a(1) */
    seq_t prev1 = 4;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t tri = (seq_t)i * ((seq_t)i + 1) / 2;
        seq_t cur = prev1 + tri * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
