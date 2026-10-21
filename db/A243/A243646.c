/*
 * A243646  Number of ways three L-tiles can be placed on an n X n square.
 * https://oeis.org/A243646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6-6*n^5-6*n^4+88*n^3-73*n^2-310*n+426)/6 for n>=4,
 *            a(n) = 0 for n<4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* indices 0..3 give 0 */

    seq_t s_n = (seq_t)n;
    seq_t n2   = s_n * s_n;           /* n^2 */
    seq_t n3   = n2   * s_n;           /* n^3 */
    seq_t n4   = n3   * s_n;           /* n^4 */
    seq_t n5   = n4   * s_n;           /* n^5 */
    seq_t n6   = n5   * s_n;           /* n^6 */

    /* Split the numerator into two positive parts so subtraction
       never underflows for unsigned types. */
    seq_t A = n6 + 88 * n3 + (seq_t)426;                /* n^6 + 88*n^3 + 426 */
    seq_t B = 6 * n5 + 6 * n4 + 73 * n2 + 310 * s_n;    /* 6*n^5 + 6*n^4 + 73*n^2 + 310*n */

    seq_t num = A - B;          /* numerator of the formula */
    return num / 6;             /* division is exact */
}
