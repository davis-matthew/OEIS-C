/*
 * A395495  Minimum number of cells in a connected polyomino on the square
 *          grid that contains every fixed n-omino as a translated subregion.
 * https://oeis.org/A395495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2n-1 for n = 1..3, a(n) = (n^2 - n + 6)/2 for n ≥ 4
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined offset, but safe fallback */
    if (n <= 3) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)3;
            case 3: return (seq_t)5;
            default: return 0;         /* unreachable */
        }
    }
    seq_t nn = (seq_t)n;
    seq_t tmp = nn * nn - nn + (seq_t)6;
    return tmp / (seq_t)2;
}
