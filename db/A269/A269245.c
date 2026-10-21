/*
 * A269245  Number of times the digit 5 appears in the decimal expansion of
 *          n^3.
 * https://oeis.org/A269245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of times digit '5' appears in the decimal
 *            expansion of n^3
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 3806, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n^3 as a seq_t value */
    seq_t cube = (seq_t)n;
    cube *= cube;          /* square: n^2 */
    cube *= (seq_t)n;       /* cube: n^3 */

    /* count the digit '5' */
    seq_t count = 0;
    while (cube > 0) {
        if ((cube % 10) == 5) {
            count++;  /* seq_t supports ++ */
        }
        cube /= 10;
    }
    return count;
}
