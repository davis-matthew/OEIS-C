/*
 * A366816  Wiener index in diamond nanowires obtained by connecting n unit
 *          cells in a sequence.
 * https://oeis.org/A366816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (338*n^3 + 234*n^2 + 124*n)/3
 * Range:     u8 none, u16 n <= 5, u32 n <= 270, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A366816: Wiener index in diamond nanowires */

seq_t a(idx_t n)
{
    /* n is at least 1 for valid calls, but there's no harm computing n = 0 */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */

    /* Compute the numerator: 338*n^3 + 234*n^2 + 124*n */
    seq_t num = 338 * n3 + 234 * n2 + 124 * sn;

    /* Integer division by 3; the result is always exact */
    return num / 3;
}
