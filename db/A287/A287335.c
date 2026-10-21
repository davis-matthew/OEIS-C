/*
 * A287335  Nonnegative numbers k such that 3*k + 2 is a cube.
 * https://oeis.org/A287335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^3 - 9*n^2 + 3*n - 1 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 781, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type */
    seq_t m = (seq_t)n;

    /* compute m^2 and m^3 in sequence type to avoid mixing signed/unsigned */
    seq_t m2 = m * m;       /* m^2 */
    seq_t m3 = m2 * m;      /* m^3 */

    /* evaluate the polynomial exactly in seq_t */
    return 9 * m3 - 9 * m2 + 3 * m - 1;
}
