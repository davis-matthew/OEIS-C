/*
 * A269249  Number of times the digit 9 appears in the decimal expansion of
 *          n^3.
 * https://oeis.org/A269249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digit '9' in the decimal expansion of n^3
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^3 safely in seq_t */
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;

    /* Count the '9' digits */
    seq_t count = 0;
    /* Handle n == 0 explicitly: cube == 0 has no '9's */
    if (cube == 0)
        return 0;

    while (cube > 0) {
        if ((cube % 10) == 9)
            ++count;
        cube /= 10;
    }
    return count;
}
