/*
 * A242854  a(n) = 512*n^9 - 1024*n^7 + 672*n^5 - 160*n^3 + 10*n.
 * https://oeis.org/A242854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 512*n^9 - 1024*n^7 + 672*n^5 - 160*n^3 + 10*n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5, u64 n <= 69, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;            /* n^2  */
    seq_t n4  = n2 * n2;            /* n^4  */
    seq_t n6  = n4 * n2;            /* n^6  */
    seq_t n8  = n4 * n4;            /* n^8  */

    seq_t term1 = (seq_t)512 * n8 * sn;   /* 512*n^9   */
    seq_t term2 = (seq_t)1024 * n6 * sn;  /* 1024*n^7  */
    seq_t term3 = (seq_t)672  * n4 * sn;  /* 672*n^5   */
    seq_t term4 = (seq_t)160  * n2 * sn;  /* 160*n^3   */
    seq_t term5 = (seq_t)10   * sn;       /*   10*n    */

    return term1 - term2 + term3 - term4 + term5;
}
