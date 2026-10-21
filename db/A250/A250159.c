/*
 * A250159  Number of length 5+6 0..n arrays with every seven consecutive
 *          terms having the maximum of some two terms equal to the minimum of
 *          the remaining five terms.
 * https://oeis.org/A250159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/15)*n^10 + (13/4)*n^9 + (2455/84)*n^8 + (722/7)*n^7 +
 *            194*n^6 + (15671/60)*n^5 + (3617/12)*n^4 + 236*n^3 +
 *            (16861/210)*n^2 + (509/210)*n + 1 = (1/420) * (28*n^10 +
 *            1365*n^9 + 12275*n^8 + 43320*n^7 + 81480*n^6 + 109697*n^5 +
 *            126595*n^4 + 99120*n^3 + 33722*n^2 + 1018*n + 420)
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 106, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Coefficients scaled by 420 */
    if (sizeof(seq_t) <= 8) {
        /* Use 128‑bit intermediate to avoid overflow for 64‑bit seq_t */
        unsigned __int128 nn = n;
        unsigned __int128 num = 28;
        num = num * nn + 1365;
        num = num * nn + 12275;
        num = num * nn + 43320;
        num = num * nn + 81480;
        num = num * nn + 109697;
        num = num * nn + 126595;
        num = num * nn + 99120;
        num = num * nn + 33722;
        num = num * nn + 1018;
        num = num * nn + 420;
        return (seq_t)(num / 420);
    } else {
        /* seq_t is large enough for all intermediate values */
        seq_t nn = (seq_t)n;
        seq_t num = 28;
        num = num * nn + 1365;
        num = num * nn + 12275;
        num = num * nn + 43320;
        num = num * nn + 81480;
        num = num * nn + 109697;
        num = num * nn + 126595;
        num = num * nn + 99120;
        num = num * nn + 33722;
        num = num * nn + 1018;
        num = num * nn + 420;
        return num / 420;
    }
}
