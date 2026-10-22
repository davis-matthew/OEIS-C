/*
 * A304828  a(n) = 344*7^n/21 - 128/3 (n>=1).
 * https://oeis.org/A304828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 344*7^n/21 - 128/3 (n >= 1) Recurrence: a(1) = 72, a(2) =
 *            760, a(n) = 8*a(n-1) - 7*a(n-2) for n>=3
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */
    if (n == 1) return (seq_t)72;
    if (n == 2) return (seq_t)760;

    seq_t prev2 = (seq_t)72;              /* a(1) */
    seq_t prev1 = (seq_t)760;             /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 8 * prev1 - (7 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
