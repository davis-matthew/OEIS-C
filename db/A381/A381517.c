/*
 * A381517  Perimeter of the Sierpiński carpet at iteration n.
 * https://oeis.org/A381517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4/5)*(4*3^n + 8^n) Recurrence: a(n) = 11*a(n-1) -
 *            24*a(n-2) (a(0)=4, a(1)=16)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)16;

    seq_t prev2 = (seq_t)4;   /* a(0) */
    seq_t prev1 = (seq_t)16;  /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)11 - prev2 * (seq_t)24;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
