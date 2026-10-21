/*
 * A243134  a(n) = 128*n^8 - 256*n^6 + 160*n^4 - 32*n^2 + 1.
 * https://oeis.org/A243134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128*n^8 - 256*n^6 + 160*n^4 - 32*n^2 + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 139, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n4 = n2 * n2;          /* n^4 */
    seq_t n6 = n4 * n2;          /* n^6 */
    seq_t n8 = n4 * n4;          /* n^8 */

    seq_t res = 128 * n8;        /* 128*n^8 */
    res -= 256 * n6;             /* -256*n^6 */
    res += 160 * n4;             /* +160*n^4 */
    res -= 32 * n2;              /* -32*n^2 */
    res += 1;                    /* +1 */
    return res;
}
