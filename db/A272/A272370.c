/*
 * A272370  Number of geometrically inscriptible regular polygons with fewer
 *          than 2^n + 1 sides.
 * https://oeis.org/A272370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*(n+2)/2 if n < 32 * a(n) = 32*n - 497 if n >= 32
 *            (valid up to 2^33)
 * Range:     u8 n <= 22, u16 n <= 2063, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term (0). */
    if (n == 0)
        return (seq_t)0;

    if (n < 32) {
        seq_t nn = (seq_t)n;
        return (nn - 1) * (nn + 2) / 2;
    } else {
        /* For n >= 32 the sequence grows linearly with slope 32. */
        seq_t nn = (seq_t)n;
        return 32 * nn - 497;
    }
}
