/*
 * A319576  a(n) = (4/15)*n*(n - 1)*(n^3 - 9*n^2 + 26*n - 9).
 * https://oeis.org/A319576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4/15)*n*(n-1)*(n^3 - 9*n^2 + 26*n - 9)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 112, u64 n <= 9291,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2u) return (seq_t)0;
    seq_t rn = (seq_t)n;
    seq_t sn = rn - (seq_t)1;
    seq_t tn = rn * rn * rn - (seq_t)9 * rn * rn + (seq_t)26 * rn - (seq_t)9;

    /* cancel factor 3 */
    if (rn % 3u == 0) {
        rn /= 3u;
    } else if (sn % 3u == 0) {
        sn /= 3u;
    } else {
        tn /= 3u;
    }

    /* cancel factor 5 */
    if (rn % 5u == 0) {
        rn /= 5u;
    } else if (sn % 5u == 0) {
        sn /= 5u;
    } else {
        tn /= 5u;
    }

    /* multiply by 4 and remaining factors */
    return rn * sn * tn * (seq_t)4;
}
