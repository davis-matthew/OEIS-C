/*
 * A254657  Numbers of words on alphabet {0,1,...,8} with no subwords ii,
 *          where i is from {0,1,2}.
 * https://oeis.org/A254657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 6*a(n-2), with a(0) = 1, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)9;   /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = ((seq_t)8 * prev1) + ((seq_t)6 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
