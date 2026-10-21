/*
 * A269461  Number of length-n 0..2 arrays with no repeated value equal to the
 *          previous repeated value.
 * https://oeis.org/A269461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 8*a(n-3) a(1) = 3, a(2) = 9, a(3) =
 *            24 (offset 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)    /* not defined for n=0, return 0 */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)9;
    if (n == 3)
        return (seq_t)24;

    /* last three values: x = a(n-1), y = a(n-2), z = a(n-3) */
    seq_t x = (seq_t)24;  /* a(3) */
    seq_t y = (seq_t)9;   /* a(2) */
    seq_t z = (seq_t)3;   /* a(1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t ai = (seq_t)3 * x + (seq_t)2 * y - (seq_t)8 * z;
        z = y;
        y = x;
        x = ai;
    }
    return x; /* a(n) */
}
