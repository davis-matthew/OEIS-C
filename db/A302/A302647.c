/*
 * A302647  a(n) = (2*n^2*(n^2 - 3) - (2*n^2 + 1)*(-1)^n + 1)/64.
 * https://oeis.org/A302647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n-2)*(n+2)/32 for even n, * a(n) = (n^2 - 1)^2/32
 *            for odd n. This is equivalent to * a(n) = (2*n^2*(n^2-3) -
 *            (2*n^2+1)*(-1)^n + 1)/64
 * Range:     u8 n <= 9, u16 n <= 38, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the simplified forms to avoid overflow on small seq_t sizes. */
    seq_t result;
    if ((n & 1) == 0) {          /* n even */
        seq_t m = (seq_t)(n / 2);            /* n = 2*m */
        seq_t m2 = m * m;                    /* m^2 */
        result = (m2 * (m2 - (seq_t)1)) / (seq_t)2;   /* m^2*(m^2-1)/2 */
    } else {                     /* n odd */
        seq_t m = (seq_t)((n - 1) / 2);        /* n = 2*m+1 */
        seq_t m2 = m * m;                      /* m^2 */
        seq_t m2p = (m + (seq_t)1) * (m + (seq_t)1);/* (m+1)^2 */
        result = (m2 * m2p) / (seq_t)2;        /* m^2*(m+1)^2/2 */
    }
    return result;
}
