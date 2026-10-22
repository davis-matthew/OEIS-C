/*
 * A321988  Number of integer triples (x,y,z) with x,y,z>0 and 4*x+6*y+3*n*z <
 *          12*n.
 * https://oeis.org/A321988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of positive integer triples (x,y,z) with 4*x + 6*y
 *            + 3*n*z < 12*n
 * Range:     u8 n <= 10, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = 0;
    for (idx_t z = 1; z <= 3; ++z) {
        /* C = 3*n*(4 - z)  -> 3n(4-z) */
        seq_t C = 3 * (seq_t)n * (4 - z);
        if (C <= 7) continue;             /* no positive solutions */
        seq_t xmax = (C - 7) / 4;          /* max x with 4x+6 <= C-1 */
        for (idx_t x = 1; x <= xmax; ++x) {
            /* y_max = floor((C - 1 - 4*x)/6) */
            seq_t y_max = (C - 1 - 4 * (seq_t)x) / 6;
            result += y_max;
        }
    }
    return result;
}
