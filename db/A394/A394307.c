/*
 * A394307  Number of induced cubes in the n X n zebra graph.
 * https://oeis.org/A394307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*(n^2-10*n+31) for n > 9. Values for n = 1..9 are:
 *            1,4,9,24,49,88,151,236,351
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 0)
        return 0; /* offset start: n>=1 */
    if (n <= 9) {
        static const seq_t tbl[9] = {
            (seq_t)1,(seq_t)4,(seq_t)9,(seq_t)24,(seq_t)49,
            (seq_t)88,(seq_t)151,(seq_t)236,(seq_t)351
        };
        return tbl[n-1];
    }
    seq_t nn = (seq_t)n;
    seq_t term = nn*nn - (seq_t)10*nn + (seq_t)31;
    return (seq_t)16 * term;
}
