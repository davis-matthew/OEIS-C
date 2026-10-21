/*
 * A251624  Numbers n such that the octagonal numbers N(n), N(n+1) and N(n+2)
 *          sum to another octagonal number.
 * https://oeis.org/A251624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2703*a(n-1) - 2703*a(n-2) + a(n-3) with a(1)=278,
 *            a(2)=752958, a(3)=2034494038 (offset 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 1) return (seq_t)278;
    if (n == 2) return (seq_t)752958;
    if (n == 3) return (seq_t)2034494038;

    /* recurrence using constant memory */
    seq_t a1 = (seq_t)278;
    seq_t a2 = (seq_t)752958;
    seq_t a3 = (seq_t)2034494038;
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i) {
        curr = (seq_t)2703 * a3 - (seq_t)2703 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }
    return a3;
}
