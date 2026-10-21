/*
 * A276293  Number of n X 2 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,1) or (0,-1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A276293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-3) for n>4, with a(1)=1,
 *            a(2)=4, a(3)=11, a(4)=31
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)11;
    if (n == 4) return (seq_t)31;

    seq_t a2 = (seq_t)4;   /* a(n-3) when n = 5 */
    seq_t a3 = (seq_t)11;  /* a(n-2) */
    seq_t a4 = (seq_t)31;  /* a(n-1) */
    seq_t new_val;

    for (idx_t i = 5; i <= n; ++i)
    {
        new_val = (seq_t)(((unsigned __int128)4 * a4)
                          - ((unsigned __int128)4 * a3)
                          + ((unsigned __int128)2 * a2));
        a2 = a3;
        a3 = a4;
        a4 = new_val;
    }
    return a4;
}
