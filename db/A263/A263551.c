/*
 * A263551  Number of (n+1) X (1+1) 0..4 arrays with each row and column
 *          divisible by 7, read as a base-5 number with top and left being
 *          the most significant digits.
 * https://oeis.org/A263551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 8*a(n-3); Initial values: a(1)=4,
 *            a(2)=10, a(3)=40
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)4;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)40;

    seq_t x0 = (seq_t)4;   /* a(n-3)   */
    seq_t x1 = (seq_t)10;  /* a(n-2)   */
    seq_t x2 = (seq_t)40;  /* a(n-1)   */
    seq_t term;

    for (idx_t i = 4; i <= n; ++i) {
        /* term = 4*a(n-1) + 2*a(n-2) - 8*a(n-3) */
        term = x2 * (seq_t)4;
        term += x1 * (seq_t)2;
        term -= x0 * (seq_t)8;

        /* slide window */
        x0 = x1;
        x1 = x2;
        x2 = term;
    }
    return x2;
}
