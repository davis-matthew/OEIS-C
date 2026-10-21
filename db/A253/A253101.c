/*
 * A253101  a(n) = A253100(2^n-1).
 * https://oeis.org/A253101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 2*a(n-3) (n > 3) Initial values:
 *            a(0)=1, a(1)=6, a(2)=24, a(3)=96
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)24;
    if (n == 3) return (seq_t)96;

    seq_t a0 = (seq_t)1,  /* a(n-3)  */
        a1 = (seq_t)6,   /* a(n-2)  */
        a2 = (seq_t)24,  /* a(n-1)  */
        a3 = (seq_t)96;  /* a(n)    */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 6 * a3 - 9 * a2 + 2 * a1;  /* a(i)   */
        a1 = a2;  /* shift window */
        a2 = a3;
        a3 = next;
    }
    return a3;
}
