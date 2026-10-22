/*
 * A304960  Number of business cards required to build an origami level n
 *          Mosely snowflake sponge.
 * https://oeis.org/A304960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 90*a(n-2), a(0) = 12, a(1) = 168
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)12;
    if (n == 1) return (seq_t)168;

    seq_t prev2 = (seq_t)12;   /* a(0) */
    seq_t prev1 = (seq_t)168;  /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)23 * prev1 - (seq_t)90 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
