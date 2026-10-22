/*
 * A395903  a(n) = n^8 + 60*n^7 + 1490*n^6 + 19800*n^5 + 151761*n^4 +
 *          671580*n^3 + 1609180*n^2 + 1741200*n + 448448.
 * https://oeis.org/A395903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 + 60*n^7 + 1490*n^6 + 19800*n^5 + 151761*n^4 +
 *            671580*n^3 + 1609180*n^2 + 1741200*n + 448448
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 248, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* cast the index once to seq_t   */
    seq_t res = 1;                /* start with coefficient of n^8   */
    res = res * nn + 60;          /* add coefficient of n^7         */
    res = res * nn + 1490;        /* add coefficient of n^6         */
    res = res * nn + 19800;       /* add coefficient of n^5         */
    res = res * nn + 151761;      /* add coefficient of n^4         */
    res = res * nn + 671580;      /* add coefficient of n^3         */
    res = res * nn + 1609180;     /* add coefficient of n^2         */
    res = res * nn + 1741200;     /* add coefficient of n^1         */
    res = res * nn + 448448;      /* add constant term               */
    return res;
}
