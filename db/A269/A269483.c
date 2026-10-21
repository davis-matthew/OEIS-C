/*
 * A269483  a(n) = n^12 - n^11 + n^9 - n^8 + n^6 - n^4 + n^3 - n + 1.
 * https://oeis.org/A269483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^12 - n^11 + n^9 - n^8 + n^6 - n^4 + n^3 - n + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v  = (seq_t)n;      /* n */
    seq_t p2 = v * v;         /* n^2 */
    seq_t p3 = p2 * v;        /* n^3 */
    seq_t p4 = p2 * p2;       /* n^4 */
    seq_t p6 = p3 * p3;       /* n^6 */
    seq_t p8 = p4 * p4;       /* n^8 */
    seq_t p9 = p8 * v;        /* n^9 */
    seq_t p11 = p8 * p3;      /* n^11 */
    seq_t p12 = p6 * p6;      /* n^12 */

    seq_t res = p12;
    res -= p11;
    res += p9;
    res -= p8;
    res += p6;
    res -= p4;
    res += p3;
    res -= v;
    res += (seq_t)1;
    return res;
}
