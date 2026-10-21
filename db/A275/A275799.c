/*
 * A275799  Number of inequivalent (modulo C_4 rotations) square n X n grids
 *          with squares coming in two colors and three squares have one of
 *          the colors.
 * https://oeis.org/A275799
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6-3*n^4+2*n^2)/24 for n even * a(n) =
 *            (n^6-3*n^4+5*n^2-3)/24 for n odd * This implements A275799 using
 *            the closed‑form expressions
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* behaviour below offset is irrelevant */

    seq_t tn  = (seq_t)n;                  /* n as seq_t */
    seq_t nsq = tn * tn;                  /* n^2 */
    seq_t n4  = nsq * nsq;                /* n^4 */
    seq_t n6  = n4 * nsq;                 /* n^6 */

    seq_t num;
    if ((tn & 1) == 0) {                 /* n even */
        num = n6 - 3 * n4 + 2 * nsq;
    } else {                             /* n odd */
        num = n6 - 3 * n4 + 5 * nsq - 3;
    }
    return num / 24;                      /* exact division */
}
