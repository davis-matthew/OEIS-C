/*
 * A269243  Number of times the digit 3 appears in the decimal expansion of
 *          n^3.
 * https://oeis.org/A269243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digit 3 in the decimal expansion of n^3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute n^3 in an intermediate type that big enough for the tests */
    unsigned __int128 cube = (unsigned __int128)n * n * n;
    unsigned int count3 = 0;
    while (cube > 0) {
        if ((unsigned int)(cube % 10) == 3) {
            count3++;
        }
        cube /= 10;
    }
    /* cube == 0 gives count3 == 0, which is correct for n == 0 */
    return (seq_t)count3;
}
