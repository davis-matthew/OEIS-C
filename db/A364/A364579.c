/*
 * A364579  Fifth Lie-Betti number of a path graph on n vertices.
 * https://oeis.org/A364579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^5 + 30*n^4 - 145*n^3 - 270*n^2 + 2424*n - 3360)/120
 *            for n >= 5, a(1)=0, a(2)=0, a(3)=1, a(4)=11
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 214, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) {
        if (n < 3) return 0;
        if (n == 3) return 1;
        return (seq_t)11; /* n == 4 */
    }

    unsigned __int128 v = (unsigned __int128)n;
    unsigned __int128 num = v * v * v * v * v;          /* n⁵ */
    num += (unsigned __int128)30 * v * v * v * v;       /* +30·n⁴ */
    num -= (unsigned __int128)145 * v * v * v;          /* -145·n³ */
    num -= (unsigned __int128)270 * v * v;             /* -270·n² */
    num += (unsigned __int128)2424 * v;                /* +2424·n */
    num -= (unsigned __int128)3360;                    /* -3360 */
    num /= (unsigned __int128)120;                     /* divide by 120 */

    return (seq_t)num;
}
