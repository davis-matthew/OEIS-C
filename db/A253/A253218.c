/*
 * A253218  Number of n X 3 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 2 and every value
 *          within 2 of its king move distance from the upper left and every
 *          value increasing by 0 or 1 with every step right, diagonally se or
 *          down.
 * https://oeis.org/A253218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n≤3, a(4) = 19, and a(n) = 40*n^2 - 279*n + 497 for
 *            n≥5
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)3) {
        return (seq_t)1;
    }
    if (n == (idx_t)4) {
        return (seq_t)19;
    }
    seq_t sn = (seq_t)n;
    seq_t res = sn * sn;          /* sn^2 */
    res = res * (seq_t)40;        /* 40*sn^2 */
    res = res - (seq_t)279 * sn;  /* -279*sn */
    res = res + (seq_t)497;       /* +497 */
    return res;
}
